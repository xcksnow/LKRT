#include <iostream>
#include <Windows.h>
#include <vector>
#include <Iphlpapi.h>
#include <string>
#include <Lm.h>

#pragma comment(lib, "Netapi32.lib")

using namespace std;

class Enumerate {
private:

	struct SystemInfo {
		string pcName;
		string osName;
		string osVersion;
		int osArchitecture;
		int ram;
		int cpuCores;
		string mac;
		vector<string> users;
		string baseboard;
		int diskSize;
		int diskFree;
	};

public:
	string getComputerName() {
		char computerName[256];
		DWORD size = sizeof(computerName);
		GetComputerNameA(computerName, &size);
		return computerName;
	}

	string getWindowsVersion() {
		OSVERSIONINFOEX osvi;
		ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		DWORDLONG dwlConditionMask = 0;
		VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
		VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);

		osvi.dwMajorVersion = 0;
		osvi.dwMinorVersion = 0;

		if (VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask)) {
			if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0)
				return "Windows 10";
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3)
				return "Windows 8.1";
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2)
				return "Windows 8";
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1)
				return "Windows 7";
			else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
				return "Windows Vista";
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
				return "Windows Server 2003";
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
				return "Windows XP";
			else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
				return "Windows 2000";
			else
				return "Uknown Version";
		}
		else {
			return "Error" + to_string(GetLastError());
		}
	}

	int getArchitecture() {
		if(sizeof(void*) == 8) {
			return 64;
		}
		else {
			return 32;
		}
	}

	int getRam() {
		MEMORYSTATUSEX memInfo;
		memInfo.dwLength = sizeof(memInfo);
		GlobalMemoryStatusEx(&memInfo);
		return memInfo.ullTotalPhys / 1024 / 1024;
	}

	int getCpuCores() {
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		return sysInfo.dwNumberOfProcessors;
	}

	string getMac() {
		PIP_ADAPTER_INFO pAdapterInfo;
		ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
		pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));
		if (pAdapterInfo == NULL) {
			return "";
		}

		if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
			free(pAdapterInfo);
			pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
			if (pAdapterInfo == NULL) {
				return "";
			}
		}

		if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != NO_ERROR) {
			free(pAdapterInfo);
			return "";
		}

		std::stringstream macStream;
		for (PIP_ADAPTER_INFO pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next) {
			macStream << std::hex << std::setfill('0');
			for (UINT i = 0; i < pAdapter->AddressLength; i++) {
				if (i > 0)
					macStream << "-";
				macStream << std::setw(2) << (int)pAdapter->Address[i];
			}
		}

		free(pAdapterInfo);
		return macStream.str();
	}

	int getDiskSize() {
		ULARGE_INTEGER freeBytesAvailable;
		ULARGE_INTEGER totalNumberOfBytes;
		ULARGE_INTEGER totalNumberOfFreeBytes;
		GetDiskFreeSpaceEx(L"C:\\", &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes);
		return totalNumberOfBytes.QuadPart / 1024 / 1024 / 1024;
	}

	int getDiskFree() {
		ULARGE_INTEGER freeBytesAvailable;
		ULARGE_INTEGER totalNumberOfBytes;
		ULARGE_INTEGER totalNumberOfFreeBytes;
		GetDiskFreeSpaceEx(L"C:\\", &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes);
		return totalNumberOfFreeBytes.QuadPart / 1024 / 1024 / 1024;
	}

	vector<string> getAllUsers() {
		std::vector<std::string> users;

		DWORD dwLevel = 0;
		DWORD dwFilter = FILTER_NORMAL_ACCOUNT;
		LPUSER_INFO_0 pBuf = NULL;
		LPUSER_INFO_0 pTmpBuf;
		DWORD dwEntriesRead = 0;
		DWORD dwTotalEntries = 0;
		DWORD dwResumeHandle = 0;
		DWORD dwStatus;

		do {
			dwStatus = NetUserEnum(NULL, dwLevel, dwFilter, (LPBYTE*)&pBuf, MAX_PREFERRED_LENGTH, &dwEntriesRead, &dwTotalEntries, &dwResumeHandle);

			if (dwStatus == NERR_Success || dwStatus == ERROR_MORE_DATA) {
				if (pBuf != NULL) {
					pTmpBuf = pBuf;
					for (DWORD i = 0; i < dwEntriesRead; i++) {
						std::wstring wstrTemp(pTmpBuf->usri0_name);
						std::string strTemp(wstrTemp.begin(), wstrTemp.end());
						users.push_back(strTemp);
						pTmpBuf++;
					}
					NetApiBufferFree(pBuf);
					pBuf = NULL;
				}
			}
			else {
				printf("Error al llamar a NetUserEnum: %u\n", dwStatus);
			}
		} while (dwStatus == ERROR_MORE_DATA);

		return users;
	}

	string getBaseboard() {
		string baseboard;
		HKEY hKey;
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
			char vendor[256];
			DWORD size = sizeof(vendor);
			RegQueryValueEx(hKey, L"BaseBoardProduct", NULL, NULL, (LPBYTE)&vendor, &size);
			baseboard = vendor;
		}	
		return baseboard;
	}

	string systeminfoToString(SystemInfo sysInfo) {
		string info;
		info += "PCName:" + sysInfo.pcName + "!";
		info += "OSName: " + sysInfo.osName + "!";
		info += "OSVersion: " + sysInfo.osVersion + "!";
		info += "OSArchitecture: " + to_string(sysInfo.osArchitecture) + "!";
		info += "RAM: " + to_string(sysInfo.ram) + "MB!";
		info += "CPU Cores: " + to_string(sysInfo.cpuCores) + "!";
		info += "MAC: " + sysInfo.mac + "!";
		info += "Users: ";
		for (int i = 0; i < sysInfo.users.size(); i++) {
			info += sysInfo.users[i] + ", ";
		}
		info += "!";
		info += "Baseboard: " + sysInfo.baseboard + "!";
		info += "Disk Size: " + to_string(sysInfo.diskSize) + " GB!";
		info += "Disk Free: " + to_string(sysInfo.diskFree) + " GB!";
		return info;
	}

	string enumerateSystem() {
		SystemInfo sysInfo;
		sysInfo.pcName = getComputerName();
		sysInfo.osName = "Windows";
		sysInfo.osVersion = getWindowsVersion();
		sysInfo.osArchitecture = getArchitecture();
		sysInfo.ram = getRam();
		sysInfo.cpuCores = getCpuCores();
		sysInfo.mac = getMac();
		sysInfo.users = getAllUsers();
		sysInfo.baseboard = getBaseboard();
		sysInfo.diskSize = getDiskSize();
		sysInfo.diskFree = getDiskFree();
		return systeminfoToString(sysInfo);
	}

	
};