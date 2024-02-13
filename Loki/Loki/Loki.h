#pragma once
#include <Windows.h>
#include <urlmon.h>
#include <curl/curl.h>
#include "resource.h"

class LokiCore{
	public:

		int init() {
			// Inicializar libcurl
			CURL* curl;
			CURLcode res;

			curl_global_init(CURL_GLOBAL_DEFAULT);
			curl = curl_easy_init();

			if (curl) {
				// Establecer la URL de destino
				curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.116");

				// Configurar la solicitud como POST
				curl_easy_setopt(curl, CURLOPT_POST, 1L);

				// Configurar los datos en el cuerpo de la solicitud
				const char* post_data = R"([ "name": "montecarlos" ])";
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

				// Realizar la solicitud POST
				res = curl_easy_perform(curl);

				// Verificar si la solicitud fue exitosa
				if (res != CURLE_OK)
					fprintf(stderr, "Error en la solicitud: %s\n", curl_easy_strerror(res));

				// Limpiar y cerrar libcurl
				curl_easy_cleanup(curl);
			}

			// Limpiar libcurl global
			curl_global_cleanup();


			while (true) {
				OutputDebugString(L"Hello World\n");
				SleepImplant(5000);
			}

			return 0;
		}

	private:

		bool SleepImplant(DWORD dwMilliseconds) {
			HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			if (hEvent == NULL) {
				return false;
			}
			HANDLE hTimer = NULL;
			LARGE_INTEGER liDueTime;
			liDueTime.QuadPart = -10000LL * dwMilliseconds;
			hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
			if (hTimer == NULL) {
				CloseHandle(hEvent);
				return false;
			}
			if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, FALSE)) {
				CloseHandle(hEvent);
				CloseHandle(hTimer);
				return false;
			}
			if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0) {
				CloseHandle(hEvent);
				CloseHandle(hTimer);
				return false;
			}
			CloseHandle(hEvent);
			CloseHandle(hTimer);
			return true;
		}
};