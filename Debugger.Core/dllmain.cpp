// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#define HookDll  extern "C" __declspec(dllexport)

#include "Exported.h"

#include "ImportMethodsUpdater.h"
#include "WapperMethods.h"
#include "DataSync.h"

SYS_GetProcAddress originalGetProcAddress = NULL;
DataSync* dataSync = NULL;
HANDLE semaphoreEmpty = NULL;
HANDLE semaphoreFull = NULL;

void InitSemaphore()
{
	semaphoreEmpty = CreateSemaphore(NULL,1,1,TEXT("DebuggerEmpty"));
	semaphoreFull = CreateSemaphore(NULL,0,1,TEXT("DebuggerFull"));
}

void GetOriginalGetProcwessAddress()
{
	HMODULE kernel32 = GetModuleHandle(TEXT("KERNEL32.dll"));
	originalGetProcAddress=(SYS_GetProcAddress)GetProcAddress(kernel32,"GetProcAddress"); 

}
HookDll void Test()
{
	//MessageBox(NULL,TEXT("Messages show here"),TEXT("Test"),MB_OK);
	ImportMethodsUpdater updater;
	GetOriginalGetProcwessAddress();
	//
	//DWORD WSASend_NEWAddress = (DWORD)WSASend_NEW;
	//updater.getMappings()->insert(make_pair("WSASend",WSASend_NEWAddress));
	////updater.updateImportAPIs();

	//DWORD SendMessageW_NEWAddress = (DWORD)SendMessageW_NEW;
	//updater.getMappings()->insert(make_pair("SendMessageW",SendMessageW_NEWAddress));
	////updater.updateImportAPIs();

	//DWORD HttpSendRequestW_NEWAddress = (DWORD)HttpSendRequestW_NEW;
	//updater.getMappings()->insert(make_pair("HttpSendRequestW",HttpSendRequestW_NEWAddress));
	////updater.updateImportAPIs();

	//DWORD InternetConnectW_NEWAddress = (DWORD)InternetConnectW_NEW;
	//updater.getMappings()->insert(make_pair("InternetConnectW",InternetConnectW_NEWAddress));

	//DWORD GetProcAddress_NEWAddress = (DWORD)GetProcAddress_NEW;
	//updater.getMappings()->insert(make_pair("GetProcAddress",GetProcAddress_NEWAddress));


	DWORD WSASendDisconnect_NEWAddress = (DWORD)WSASendDisconnect_NEW;
	updater.getMappings()->insert(make_pair("WSASendDisconnect",WSASendDisconnect_NEWAddress));

	DWORD recv_NEWAddress = (DWORD)recv_NEW;
	updater.getMappings()->insert(make_pair("recv",recv_NEWAddress));

	DWORD send_NEWAddress = (DWORD)send_NEW;
	updater.getMappings()->insert(make_pair("send",send_NEWAddress));
	
	DWORD connect_NEWAddress = (DWORD)connect_NEW;
	updater.getMappings()->insert(make_pair("connect",connect_NEWAddress));

	DWORD closesocket_NEWAddress = (DWORD)closesocket_NEW;
	updater.getMappings()->insert(make_pair("closesocket",closesocket_NEWAddress));

	updater.updateImportAPIs();
	//updater.getAllImportMethods();
	dataSync = new DataSync();
	InitSemaphore();
	return;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		 DisableThreadLibraryCalls(hModule);
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		if(semaphoreEmpty)
		{
			CloseHandle(semaphoreEmpty);
		}

		if(semaphoreFull)
		{
			CloseHandle(semaphoreFull);
		}
		break;
	}
	return TRUE;
}
