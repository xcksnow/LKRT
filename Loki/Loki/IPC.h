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

    string readStringADS() {
		const char* filename = "C:\\Users\\Public\\Music\\file.txt";
		const char* streamname = "hiddenstream";

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