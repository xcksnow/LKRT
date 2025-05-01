#include <windows.h>
#include <taskschd.h>
#include <comutil.h>
#include <comdef.h>
#include <iostream>

using namespace std;

class TaskCreator {
public:
    HRESULT CreateScheduledTask(const LPCWSTR& command, const LPCWSTR& taskName, const LPCWSTR& parameters, const DWORD triggerIntervalSeconds) {

        // Initialize COM library
        HRESULT hr = CoInitialize(NULL);
        if (FAILED(hr)) {
            cerr << "Error initializing COM library: " << hr << endl;
            return hr;
        }

        // Create an instance of the Task Service
        ITaskService* pService = NULL;
        hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER,
            IID_ITaskService, (void**)&pService);
        if (FAILED(hr)) {
            cerr << "Error creating Task Service instance: " << hr << endl;
            CoUninitialize();
            return hr;
        }

        // Connect to the task service
        hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
        if (FAILED(hr)) {
            cerr << "Error connecting to Task Service: " << hr << endl;
            pService->Release();
            CoUninitialize();
            return hr;
        }

        // Get the root folder
        ITaskFolder* pRootFolder = NULL;
        hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
        if (FAILED(hr)) {
            cerr << "Error getting root folder: " << hr << endl;
            pService->Release();
            CoUninitialize();
            return hr;
        }

        // Create the task
        ITaskDefinition* pTask = NULL;
        hr = pService->NewTask(0, &pTask);
        if (FAILED(hr)) {
            cerr << "Error creating task: " << hr << endl;
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        // Set the task settings
        IRegistrationInfo* pRegInfo = NULL;
        hr = pTask->get_RegistrationInfo(&pRegInfo);
        if (FAILED(hr)) {
            cerr << "Error getting registration info: " << hr << endl;
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        hr = pRegInfo->put_Description(_bstr_t(L"Task Scheduler API Task"));
        if (FAILED(hr)) {
            cerr << "Error setting task description: " << hr << endl;
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        // Set the task trigger
        ITriggerCollection* pTriggerCollection = NULL;
        hr = pTask->get_Triggers(&pTriggerCollection);
        if (FAILED(hr)) {
            cerr << "Error getting trigger collection: " << hr << endl;
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        ITrigger* pTrigger = NULL;
        hr = pTriggerCollection->Create(TASK_TRIGGER_DAILY, &pTrigger);
        if (FAILED(hr)) {
            cerr << "Error creating trigger: " << hr << endl;
            pTriggerCollection->Release();
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        IRepetitionPattern* pRepetitionPattern = NULL;
        hr = pTrigger->get_Repetition(&pRepetitionPattern);
        if (FAILED(hr)) {
            cerr << "Error getting repetition pattern: " << hr << endl;
            pTrigger->Release();
            pTriggerCollection->Release();
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        hr = pRepetitionPattern->put_Interval(_bstr_t((wstring(L"PT") + to_wstring(triggerIntervalSeconds) + L"S").c_str()));
        if (FAILED(hr)) {
            cerr << "Error setting repetition interval: " << hr << endl;
            pRepetitionPattern->Release();
            pTrigger->Release();
            pTriggerCollection->Release();
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        hr = pRepetitionPattern->put_Duration(_bstr_t(L"PT24H"));
        if (FAILED(hr)) {
            cerr << "Error setting repetition duration: " << hr << endl;
            pRepetitionPattern->Release();
            pTrigger->Release();
            pTriggerCollection->Release();
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        // Set the task action
        IActionCollection* pActionCollection = NULL;
        hr = pTask->get_Actions(&pActionCollection);
        if (FAILED(hr)) {
            cerr << "Error getting action collection: " << hr << endl;
            pRepetitionPattern->Release();
            pTrigger->Release();
            pTriggerCollection->Release();
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        IAction* pAction = NULL;
        hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
        if (FAILED(hr)) {
            cerr << "Error creating action: " << hr << endl;
            pActionCollection->Release();
            pRepetitionPattern->Release();
            pTrigger->Release();
            pTriggerCollection->Release();
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        IExecAction* pExecAction = NULL;
        hr = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
        if (FAILED(hr)) {
            cerr << "Error querying interface for IExecAction: " << hr << endl;
            pAction->Release();
            pActionCollection->Release();
            pRepetitionPattern->Release();
            pTrigger->Release();
            pTriggerCollection->Release();
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        hr = pExecAction->put_Path(_bstr_t(command));
        if (FAILED(hr)) {
            cerr << "Error setting action path: " << hr << endl;
            pExecAction->Release();
            pAction->Release();
            pActionCollection->Release();
            pRepetitionPattern->Release();
            pTrigger->Release();
            pTriggerCollection->Release();
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        hr = pExecAction->put_Arguments(_bstr_t(parameters));
        if (FAILED(hr)) {
            cerr << "Error setting action parameters: " << hr << endl;
            pExecAction->Release();
            pAction->Release();
            pActionCollection->Release();
            pRepetitionPattern->Release();
            pTrigger->Release();
            pTriggerCollection->Release();
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        IRegisteredTask* pRegisteredTask = NULL;
        // Save the task
        hr = pRootFolder->RegisterTaskDefinition(_bstr_t(taskName), pTask, TASK_CREATE_OR_UPDATE, _variant_t(), _variant_t(), TASK_LOGON_INTERACTIVE_TOKEN, _variant_t(L""), &pRegisteredTask);
        if (FAILED(hr)) {
            cerr << "Error registering task definition: " << hr << endl;
            pExecAction->Release();
            pAction->Release();
            pActionCollection->Release();
            pRepetitionPattern->Release();
            pTrigger->Release();
            pTriggerCollection->Release();
            pRegInfo->Release();
            pTask->Release();
            pRootFolder->Release();
            pService->Release();
            CoUninitialize();
            return hr;
        }

        // Clean up
        pExecAction->Release();
        pAction->Release();
        pActionCollection->Release();
        pRepetitionPattern->Release();
        pTrigger->Release();
        pTriggerCollection->Release();
        pRegInfo->Release();
        pTask->Release();
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return S_OK;
    }
};


