// SMDebugger.Msg.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ProcessHelper.h"
#include <Shellapi.h>
#include "Imported.h"
#include "Inject.h"
#include "string"
using namespace std;
void StartHook(HWND hWnd);
DWORD getProcessId();
void startDebugger();

DWORD threadId = 0;

int _tmain(int argc, _TCHAR* argv[])
{
	startDebugger();
	return 0;
}

void startDebugger()
{
	//Get SM process id
	DWORD smProcessId = getProcessId();
	if(smProcessId)
	{
		HANDLE hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, smProcessId);
		if(hTargetProcess)
		{
			string dllName = "C:\\Users\\daizhen\\VS Projects\\2008\\SMDebugger.Msg\\Debug\\Debugger.Core.dll";
			//LPWSTR pDirName = (LPWSTR)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,512);
			//DWORD length = GetCurrentDirectory(512,pDirName);

			//string dllName = "Debugger.Core.dll";
			string methodName = "Test";
			Inject(hTargetProcess,dllName.c_str(),methodName.c_str());
		}
		else
		{
			MessageBox(NULL,TEXT("Did not open the process"),TEXT("xxxx"),MB_OK);
		}
	}
}

DWORD getProcessId()
{
	ProcessHelper helper;
	vector<PROCESSENTRY32> processes = helper.GetProcessList();

	DWORD processId = 0;
	DWORD parentProcessId = 0;

	for(int i=0;i<processes.size();i++)
	{
		PROCESSENTRY32 currentProcess = processes.at(i);
		TCHAR* processName = currentProcess.szExeFile;
		if(_tcscmp(processName,TEXT("ServiceManager.exe")) ==0)
		{
			parentProcessId = currentProcess.th32ProcessID;
			BOOL hasChildProcess = FALSE;
			//break;
			for(int j=0;j<processes.size();j++)
			{
				PROCESSENTRY32 currentProcess = processes.at(j);
				TCHAR* processName = currentProcess.szExeFile;
				if(currentProcess.th32ParentProcessID == parentProcessId)
				{
					hasChildProcess = TRUE;
					processId = currentProcess.th32ProcessID;
					vector<THREADENTRY32> threads = helper.GetThreadList(processId);
					threadId = helper.GetWindowThreadId(processId);
					break;
				}
			}


			if (!hasChildProcess)
			{
				PROCESSENTRY32 currentProcess = processes.at(i);
				processId = currentProcess.th32ProcessID;
				vector<THREADENTRY32> threads = helper.GetThreadList(processId);
				threadId = helper.GetWindowThreadId(processId);
			}

			if(processId!=0 && threadId!=0)
			{
				break;
			}
		}
	}
	return processId;
}

void StartHook(HWND hWnd)
{
	ProcessHelper helper;
	vector<PROCESSENTRY32> processes = helper.GetProcessList();

	DWORD processId = 0;
	DWORD parentProcessId = 0;

	DWORD threadId = 0;

	for(int i=0;i<processes.size();i++)
	{
		PROCESSENTRY32 currentProcess = processes.at(i);
		TCHAR* processName = currentProcess.szExeFile;
		if(_tcscmp(processName,TEXT("ServiceManager.exe")) ==0)
		{
			parentProcessId = currentProcess.th32ProcessID;
			//break;
			for(int j=0;j<processes.size();j++)
			{
				PROCESSENTRY32 currentProcess = processes.at(j);
				TCHAR* processName = currentProcess.szExeFile;
				if(currentProcess.th32ParentProcessID == parentProcessId)
				{
					processId = currentProcess.th32ProcessID;
					vector<THREADENTRY32> threads = helper.GetThreadList(processId);
					threadId = helper.GetWindowThreadId(processId);
					break;
				}
			}

			if(processId!=0 && threadId!=0)
			{
				break;
			}
		}
	}

	if(processId!=0 && threadId!=0)
	{
		//EndHook(hWnd);
		//SetHook(threadId);

	}
	else
	{
		MessageBox(NULL,TEXT("SM client not started"),TEXT("ERROR"),MB_OK);
	}
}