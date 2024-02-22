#pragma once
#include <Windows.h>
#include <iostream>
#include "Utils.h"

using namespace std;

class FileHider {
public:
    bool hideFile(string sourceFile, string destinationFile, string adsName) {
        Utils utils = Utils();
        LPCWSTR srcFile = utils.convertStringToLPCWSTR(sourceFile);

        // Open the source file for reading.
        HANDLE hSourceFile = CreateFile(srcFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hSourceFile == INVALID_HANDLE_VALUE) {
            cout << "Error: " << GetLastError() << endl;
            return false;
        }

        // Read the source file content.
        const int bufferSize = 1024;
        char buffer[bufferSize];
        DWORD bytesRead;
        string adsContent = "";
        while (ReadFile(hSourceFile, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0) {
            adsContent += buffer;
        }

        // Create ADS
        string adsPath = destinationFile + ":" + adsName;
        LPCWSTR adsPathLPCWSTR = utils.convertStringToLPCWSTR(adsPath);

        HANDLE hFile = CreateFile(adsPathLPCWSTR, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            return false;
        }

        // Write data to the Alternate Data Stream.
        DWORD dwBytesWritten = 0;
        bool bErrorFlag = WriteFile(hFile, adsContent.c_str(), strlen(adsContent.c_str()), &dwBytesWritten, NULL);
        if (FALSE == bErrorFlag) {
            return false;
        }

        // Remove the original file.
        CloseHandle(hFile);
        CloseHandle(hSourceFile);
       
        if (DeleteFile(srcFile) == 0) {
			return false;
		}
        return true;
    }

    bool unHideFile(string sourceFile, string destinationFile, string adsName) {
        LPCWSTR srcFile = Utils().convertStringToLPCWSTR(sourceFile);
        LPCWSTR dstFile = Utils().convertStringToLPCWSTR(destinationFile);
        HANDLE hFile = CreateFile(dstFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            return false;
        }

        string adsPath = sourceFile + ":" + adsName;
        LPCWSTR adsPathLPCWSTR = Utils().convertStringToLPCWSTR(adsPath);
        HANDLE hADS = CreateFile(adsPathLPCWSTR, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hADS == INVALID_HANDLE_VALUE) {
            // What error code is this?
            CloseHandle(hFile);
            return false;
        }

        const int bufferSize = 1024;
        char buffer[bufferSize];
        DWORD bytesRead, bytesWritten;
        while (ReadFile(hADS, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0) {
            WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL);
        }

        CloseHandle(hADS);
        CloseHandle(hFile);

        if (remove(adsPath.c_str()) != 0) {
            CloseHandle(hADS);
            CloseHandle(hFile);
            return false;
        }

        return true;
    }

    

};