#include <Windows.h>
#include <tchar.h>
#include <iostream>

class Installer {
public:

    static BOOL InstallService(const TCHAR* serviceName, const TCHAR* servicePath) {
        SC_HANDLE schSCManager = NULL;
        SC_HANDLE schService = NULL;
        BOOL success = FALSE;

        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
        if (schSCManager == NULL) {
            return false;
        }

        schService = CreateService(
            schSCManager,
            serviceName,
            serviceName,
            SERVICE_ALL_ACCESS,
            SERVICE_WIN32_OWN_PROCESS,
            SERVICE_AUTO_START,
            SERVICE_ERROR_NORMAL,
            servicePath,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        );

        if (schService == NULL) {
            return false;
        }

        success = TRUE;

        if (schService != NULL) {
            CloseServiceHandle(schService);
        }

        if (schSCManager != NULL) {
            CloseServiceHandle(schSCManager);
        }

        return success;
    }
};