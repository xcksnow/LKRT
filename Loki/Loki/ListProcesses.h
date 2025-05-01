#include <Windows.h>
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <TlHelp32.h>

using namespace std;

struct ProcessInfo {
	string name;
	DWORD pid;
	DWORD parentPid;
	string username;
	string imageFilePath;


};

class ListProcess {
public:
	vector<ProcessInfo> listAllProcesses() {
		vector<ProcessInfo> processes;
		HANDLE hProcessSnap;
		PROCESSENTRY32 pe32;
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hProcessSnap == INVALID_HANDLE_VALUE) {
			return processes;
		}
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (!Process32First(hProcessSnap, &pe32)) {
			CloseHandle(hProcessSnap);
			return processes;
		}
		do {
			std::wstring wstr(pe32.szExeFile);
			ProcessInfo process;
			int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
			std::string str(bufferSize, 0);
			WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], bufferSize, NULL, NULL);
			str.resize(bufferSize - 1);

			process.name = str;
			process.pid = pe32.th32ProcessID;
			process.parentPid = pe32.th32ParentProcessID;
			process.username = GetProcessUserName(pe32.th32ProcessID);
			processes.push_back(process);
		} while (Process32Next(hProcessSnap, &pe32));
		CloseHandle(hProcessSnap);
		return processes;
	}

	string GetProcessUserName(DWORD pid) {
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
		if (!hProcess) return "";
		HANDLE hToken = NULL;
		if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
			CloseHandle(hProcess);
			return "";
		}
		DWORD dwSize = 0;
		GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize);
		PTOKEN_USER pTokenUser = (PTOKEN_USER)malloc(dwSize);
		if (!pTokenUser) {
			CloseHandle(hToken);
			CloseHandle(hProcess);
			return "";
		}
		if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize)) {
			free(pTokenUser);
			CloseHandle(hToken);
			CloseHandle(hProcess);
			return "";
		}
		SID_NAME_USE SidType;
		wchar_t lpName[MAX_PATH];
		DWORD dwNameSize = MAX_PATH;
		wchar_t lpDomain[MAX_PATH];
		DWORD dwDomainSize = MAX_PATH;
		if (!LookupAccountSid(NULL, pTokenUser->User.Sid, lpName, &dwNameSize, lpDomain, &dwDomainSize, &SidType)) {
			free(pTokenUser);
			CloseHandle(hToken);
			CloseHandle(hProcess);
			return "";
		}
		wstring username(lpDomain);
		username += L"/";
		username += lpName;
		free(pTokenUser);
		CloseHandle(hToken);
		CloseHandle(hProcess);
		return string(username.begin(), username.end());  // Convert wstring to string
	}


	string processesToString(vector<ProcessInfo> processes) {
		string result = "";
		for (ProcessInfo process : processes) {
			result += "!Name: " + process.name + ",";
			result += "PID: " + to_string(process.pid) + ",";
			result += "Parent PID: " + to_string(process.parentPid) + ",";
			result += "Username: " + process.username + ",";
		}
		return result;
	}
};