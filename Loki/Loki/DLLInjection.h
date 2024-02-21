#pragma once
#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <iostream>
#include "Utils.h"

using namespace std;

class DLLInjector {
public:

    //enum ProtectionType { R = 1, W = 2, X = 4, RW = 3, RX = 5, WX = 6, RWX = 7 };


    bool DLLinjector(DWORD pid, const char* dllPath) {
        typedef LPVOID memory_buffer;

        HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        if (hProc == NULL) {
            cout << "OpenProcess() failed: " << GetLastError() << endl;
            return false;
        }

        HMODULE hKernel32 = GetModuleHandle(L"Kernel32");
        void* lb = GetProcAddress(hKernel32, "LoadLibraryA");
        
        Utils utils = Utils();
        
        // RWX
        memory_buffer allocMem = VirtualAllocEx(hProc, NULL, strlen(dllPath), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

        WriteProcessMemory(hProc, allocMem, dllPath, strlen(dllPath), NULL);
        HANDLE rThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)lb, allocMem, 0, NULL);
        if (rThread == NULL) {
            cout << "CreateRemoteThread() failed: " << GetLastError() << endl;
            return false;
        }
        CloseHandle(hProc);
        FreeLibrary(hKernel32);
        VirtualFreeEx(hProc, allocMem, strlen(dllPath), MEM_RELEASE);
        return true;
    }
};