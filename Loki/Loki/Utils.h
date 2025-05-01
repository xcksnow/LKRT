#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

using namespace std;

class Utils{
public:

    enum ProtectionType { R = 1, W = 2, X = 4, RW = 3, RX = 5, WX = 6, RWX = 7 };

    string findFilePath(const string& fileName) {
		// use GetFullPathNameA
        char buffer[MAX_PATH];
        GetFullPathNameA(fileName.c_str(), MAX_PATH, buffer, nullptr);
        return string(buffer);
	}

	int getPIDbyProcName(const string& procName) {
		int pid = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32W pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32W);
		if (Process32FirstW(hSnap, &pe32) != FALSE) {
			while (pid == 0 && Process32NextW(hSnap, &pe32) != FALSE) {
				wstring wideProcName(procName.begin(), procName.end());
				if (wcscmp(pe32.szExeFile, wideProcName.c_str()) == 0) {
					pid = pe32.th32ProcessID;
				}
			}
		}
		CloseHandle(hSnap);
		return pid;
	}

    char* MyOwnVirtualAlloc(int size, ProtectionType protectionType) {
        enum ProtectionType { R = 1, W = 2, X = 4, RW = 3, RX = 5, WX = 6, RWX = 7 };
        char dll[] = { 'w','i','n','m','m','.','d','l','l','\0' };

        DWORD protection = 0;
        char* address = nullptr;
        DWORD oldProtect;
        HMODULE victimDLL = GetModuleHandleA(dll);

        if (victimDLL == NULL) {
            victimDLL = LoadLibraryA(dll);
            if (victimDLL != NULL) {
                address = (char*)victimDLL;
            }
        }
        else {
            address = (char*)victimDLL;
        }

        char* allocatedMemory = nullptr;

        switch (protectionType) {
        case R:
            protection = PAGE_READONLY;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case W:
            protection = PAGE_WRITECOPY;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case X:
            protection = PAGE_EXECUTE;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case RW:
            protection = PAGE_READWRITE;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case RX:
            protection = PAGE_EXECUTE_READ;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case WX:
            protection = PAGE_EXECUTE_WRITECOPY;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        case RWX:
            protection = PAGE_EXECUTE_READWRITE;
            allocatedMemory = (char*)address + 2 * 4096 + 12;
            break;
        default:
            protection = PAGE_NOACCESS;
            break;
        }

        if (allocatedMemory != nullptr) {
            VirtualProtect(allocatedMemory, size, protection, &oldProtect);
        }

        return allocatedMemory;
    }

    //static std::wstring findFileDirectory(const wchar_t* filename) {
    //    wchar_t buffer[MAX_PATH];
    //    GetModuleFileNameW(nullptr, buffer, MAX_PATH); // Obtener la ruta del ejecutable
    //    wchar_t* lastSlash = wcsrchr(buffer, L'\\');

    //    if (lastSlash != nullptr) {
    //        *lastSlash = L'\0'; // Eliminar el nombre del ejecutable para obtener solo el directorio
    //        std::wstring directory(buffer);
    //        directory.append(L"\\");
    //        directory.append(filename); // Agregar el nombre del archivo al directorio
    //        return directory;
    //    }
    //    else {
    //        return L"";
    //    }
    //}


    static std::wstring findFileDirectoryW(const wchar_t* filename) {
        wchar_t buffer[MAX_PATH];
        GetFullPathNameW(filename, MAX_PATH, buffer, nullptr); 
        wchar_t* lastSlash = wcsrchr(buffer, L'\\');

        if (lastSlash != nullptr) {
            *lastSlash = L'\0';
            return std::wstring(buffer);
        }
        else {
            return L"";
        }
    }

    std::string findFileDirectory(const std::string& nombreArchivo) {
        fs::path rutaAbsoluta = fs::absolute(nombreArchivo);
        return rutaAbsoluta.string();
    }


    char* MyOwnVirtualAllocEx(DWORD processId, int size, ProtectionType protectionType) {
        enum ProtectionType { R = 1, W = 2, X = 4, RW = 3, RX = 5, WX = 6, RWX = 7 };
        char dll[] = { 'w','i','n','m','m','.','d','l','l','\0' };

        DWORD protection = 0;
        char* address = nullptr;
        DWORD oldProtect;
        HANDLE hProcess = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processId);
        if (hProcess == NULL) {
            return nullptr;
        }

        HMODULE victimDLL = GetModuleHandleA(dll);
        if (victimDLL == NULL) {
            victimDLL = LoadLibraryA(dll); 
            if (victimDLL == NULL) {
                CloseHandle(hProcess);
                return nullptr;
            }
        }

        address = (char*)victimDLL;

        switch (protectionType) {
        case R:
            protection = PAGE_READONLY;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case W:
            protection = PAGE_WRITECOPY;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case X:
            protection = PAGE_EXECUTE;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case RW:
            protection = PAGE_READWRITE;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case RX:
            protection = PAGE_EXECUTE_READ;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case WX:
            protection = PAGE_EXECUTE_WRITECOPY;
            address = (char*)address + 2 * 4096 + 12;
            break;
        case RWX:
            protection = PAGE_EXECUTE_READWRITE;
            address = (char*)address + 2 * 4096 + 12;
            break;
        default:
            protection = PAGE_NOACCESS;
            address = (char*)address + 2 * 4096 + 12;
            break;

        CloseHandle(hProcess);
		}

        if (VirtualProtectEx(hProcess, address, size, protection, &oldProtect) == 0) {
            CloseHandle(hProcess);
        }
            return (char*)address;
    }

    static LPCWSTR convertStringToLPCWSTR(const string& s) {
		wstring wideString(s.begin(), s.end());
		return wideString.c_str();
	}

    static LPWSTR convertStringToLPWSTR(const string& s) {
        wstring wideString(s.begin(), s.end());
        return const_cast<LPWSTR>(wideString.c_str());
    }

    static bool fileExists(const string& name) {
		ifstream f(name.c_str());
		return f.good();
	}

};