#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>

using namespace std;

class Utils{
public:

    enum ProtectionType { R = 1, W = 2, X = 4, RW = 3, RX = 5, WX = 6, RWX = 7 };

	int getPIDbyProcName(const string& procName) {
		int pid = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32W pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32W);
		if (Process32FirstW(hSnap, &pe32) != FALSE) {
			while (pid == 0 && Process32NextW(hSnap, &pe32) != FALSE) {
				wstring wideProcName(procName.begin(), procName.end());
				if (wcscmp(pe32.szExeFile, wideProcName.c_str()) == 0) {
					pid = pe32.th32ProcessID;
				}
			}
		}
		CloseHandle(hSnap);
		return pid;
	}

    char* MyOwnVirtualAlloc(int size, ProtectionType protectionType) {
        enum ProtectionType { R = 1, W = 2, X = 4, RW = 3, RX = 5, WX = 6, RWX = 7 };
        char dll[] = { 'w','i','n','m','m','.','d','l','l','\0' };

        DWORD protection = 0;
        char* address = nullptr;
        DWORD oldProtect;
        HMODULE victimDLL = GetModuleHandleA(dll);

        if (victimDLL == NULL) {
            victimDLL = LoadLibraryA(dll);
            if (victimDLL != NULL) {
                address = (char*)victimDLL;
            }
        }
        else {
            address = (char*)victimDLL;
        }

        char* allocatedMemory = nullptr;

        switch (protectionType) {
        case R:
            protection = PAGE_READONLY;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case W:
            protection = PAGE_WRITECOPY;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case X:
            protection = PAGE_EXECUTE;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case RW:
            protection = PAGE_READWRITE;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case RX:
            protection = PAGE_EXECUTE_READ;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case WX:
            protection = PAGE_EXECUTE_WRITECOPY;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case RWX:
            protection = PAGE_EXECUTE_READWRITE;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        default:
            protection = PAGE_NOACCESS;
            break;
        }

        if (allocatedMemory != nullptr) {
            VirtualProtect(allocatedMemory, size, protection, &oldProtect);
        }

        return allocatedMemory;
    }

    char* MyOwnVirtualAllocEx(DWORD processId, int size, ProtectionType protectionType) {
        enum ProtectionType { R = 1, W = 2, X = 4, RW = 3, RX = 5, WX = 6, RWX = 7 };
        char dll[] = { 'w','i','n','m','m','.','d','l','l','\0' };

        DWORD protection = 0;
        char* address = nullptr;
        DWORD oldProtect;
        HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processId);
        if (hProcess == NULL) {
            return nullptr;
        }

        HMODULE victimDLL = GetModuleHandleA(dll);
        if (victimDLL == NULL) {
            victimDLL = LoadLibraryA(dll); 
            if (victimDLL == NULL) {
                CloseHandle(hProcess);
                return nullptr;
            }
        }

        address = (char*)victimDLL;

        switch (protectionType) {
        case R:
            protection = PAGE_READONLY;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case W:
            protection = PAGE_WRITECOPY;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case X:
            protection = PAGE_EXECUTE;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case RW:
            protection = PAGE_READWRITE;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case RX:
            protection = PAGE_EXECUTE_READ;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case WX:
            protection = PAGE_EXECUTE_WRITECOPY;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case RWX:
            protection = PAGE_EXECUTE_READWRITE;
            address = (char*)address + 2 * 4096 + 12;
            break;
        default:
            protection = PAGE_NOACCESS;
            address = (char*)address + 2 * 4096 + 12;
            break;

        CloseHandle(hProcess);
		}

        if (VirtualProtectEx(hProcess, address, size, protection, &oldProtect) == 0) {
            CloseHandle(hProcess);
        }
            return (char*)address;
    }

    static LPCWSTR convertStringToLPCWSTR(const string& s) {
		wstring wideString(s.begin(), s.end());
		return wideString.c_str();
	}

};