#include <Windows.h>
#include <iostream>
#include <Tlhelp32.h>
#include <string>


using namespace std;

class PPIDSpoofer {
public:
	int SpoofNewProcess(string exePath, DWORD parentPID) {
        STARTUPINFOEX info = { sizeof(STARTUPINFOEX) }; // Changed sizeof(0) to sizeof(STARTUPINFOEX)
        PROCESS_INFORMATION processInfo;
        SIZE_T cbAttributeListSize = 0;
        PPROC_THREAD_ATTRIBUTE_LIST pAttributeList = NULL;
        HANDLE hParentProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, parentPID); // Open the parent process with all access

        InitializeProcThreadAttributeList(NULL, 1, 0, &cbAttributeListSize); // Get the size of the attribute list
        pAttributeList = (PPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, cbAttributeListSize);  // Allocate memory for the attribute list
        InitializeProcThreadAttributeList(pAttributeList, 1, 0, &cbAttributeListSize); // Initialize the attribute list
        UpdateProcThreadAttribute(pAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS, &hParentProcess, sizeof(HANDLE), NULL, NULL); // Update the attribute list with the parent process
        info.lpAttributeList = pAttributeList; // Set the attribute list in the startup info

        wchar_t* exePathWchar = new wchar_t[exePath.length() + 1];
        mbstowcs(exePathWchar, exePath.c_str(), exePath.length() + 1);

        // Create new process using exePathWchar
        if (!CreateProcess(exePathWchar, NULL, NULL, NULL, FALSE, EXTENDED_STARTUPINFO_PRESENT | CREATE_NO_WINDOW, NULL, NULL, reinterpret_cast<STARTUPINFO*>(&info), &processInfo)) {
			cout << "Error: " << GetLastError() << endl;
			// Store GetLastError() in a variable and return it
			DWORD error = GetLastError();
			return error;
		}
        return 0;
	}

private:
    LPWSTR convertStringToLPWSTR(const string& s) {
        wstring wideString(s.begin(), s.end());
        return const_cast<LPWSTR>(wideString.c_str());
    }

    LPCWSTR convertStringToLPCWSTR(const string& s) {
        wstring wideString(s.begin(), s.end());
        return wideString.c_str();
    }
};