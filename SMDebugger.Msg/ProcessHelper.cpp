#include "StdAfx.h"
#include "ProcessHelper.h"

HWND mainWindowForProcess;

ProcessHelper::ProcessHelper(void)
{
}

ProcessHelper::~ProcessHelper(void)
{

}

vector<PROCESSENTRY32> ProcessHelper::GetProcessList()
{
	vector<PROCESSENTRY32> processes;
	HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,NULL);

	PROCESSENTRY32 currentEntry;
	currentEntry.dwSize=sizeof(currentEntry);

	BOOL result = Process32First(snapshotHandle,&currentEntry);

	while(result)
	{
		processes.push_back(currentEntry);
		result = Process32Next(snapshotHandle,&currentEntry);
	}
	CloseHandle(snapshotHandle);
	return processes;
}




vector<THREADENTRY32> ProcessHelper::GetThreadList(DWORD processId)
{
	vector<THREADENTRY32> threads;
	HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,processId);

	THREADENTRY32 currentEntry;
	currentEntry.dwSize=sizeof(currentEntry);

	BOOL result = Thread32First(snapshotHandle,&currentEntry);

	while(result)
	{
		threads.push_back(currentEntry);
		result = Thread32Next(snapshotHandle,&currentEntry);
	}
	CloseHandle(snapshotHandle);
	return threads;
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd,DWORD lParam)
{
	DWORD mpid;
	GetWindowThreadProcessId(hwnd,&mpid);
	if (mpid == lParam)
	{
		TCHAR targetWinName[]=TEXT("HP Service Manager");

		TCHAR name[400];

		GetWindowText(hwnd,name,40);

		if(wcslen(name) >= wcslen(targetWinName))
		{
			BOOL isMainWindow = TRUE;
			for(int i=0;i<wcslen(targetWinName);i++)
			{
				if(name[i]!=targetWinName[i])
				{
					isMainWindow = FALSE;
					break;
				}
			}
			if(isMainWindow)
			{
				mainWindowForProcess=hwnd;
			}
		}
	}
	return TRUE;
}

DWORD ProcessHelper::GetWindowThreadId(DWORD processId)
{
	EnumWindows((WNDENUMPROC)EnumWindowsProc,processId);
	if(mainWindowForProcess!=NULL)
	{
		DWORD temProcessId;
		return GetWindowThreadProcessId(mainWindowForProcess,&temProcessId);
	}
	return 0;
}