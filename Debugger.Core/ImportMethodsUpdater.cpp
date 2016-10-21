#include "StdAfx.h"
#include "ImportMethodsUpdater.h"
#include "ImportDescriptorView.h"
#include <psapi.h>
#pragma comment ( lib, "psapi.lib" )

ImportMethodsUpdater::ImportMethodsUpdater(void)
{
}

ImportMethodsUpdater::~ImportMethodsUpdater(void)
{
}

void ImportMethodsUpdater::getAllImportMethods()
{
	char* methodsStr = new char[1024*1024*10];
	*methodsStr=0;
	//Get current process id
	DWORD currentProcessId = GetCurrentProcessId();
	//Get process handle
	HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION |  PROCESS_VM_READ,  FALSE, currentProcessId);
	//Define modules
	HMODULE hMods[1024];
	DWORD cbNeeded;

	//char* importFunctions = (char*)malloc(1024*1024);
	//*importFunctions=0;


	//Walk all modules 
	if( EnumProcessModules(processHandle, hMods, sizeof(hMods), &cbNeeded))
	{
		for (  int i = 0; i < (int)(cbNeeded / sizeof(HMODULE)); i++ )
		{
			HMODULE currentHandle = hMods[i];
			char moduleName[1024];
			GetModuleFileNameA(currentHandle,moduleName,1024);
			//strcat(importFunctions,moduleName);
			//strcat(importFunctions,"\r\n");

			PEFile* peFile2 = new PEFile();
			peFile2->Construct(currentHandle);

			IMAGE_IMPORT_DESCRIPTOR* currentIID = peFile2->GetImportDescriptor();
			if(currentIID != NULL)
			{
				while(currentIID->FirstThunk != 0)
				{
					ImportDescriptorView* currentView = new ImportDescriptorView(peFile2,currentIID);

					currentView->GetAllImportFunctions(peFile2,currentIID,methodsStr);
					//if(address>0)
					//{
					//	DWORD* tem = (DWORD*)address;
					//	*tem = (DWORD)MYMessageBoxW;
					//}
					currentIID++;
				}
			}
		}
	}
}
void ImportMethodsUpdater::updateImportAPIs()
{
	//Get current process id
	DWORD currentProcessId = GetCurrentProcessId();
	//Get process handle
	HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION |  PROCESS_VM_READ,  FALSE, currentProcessId);
	//Define modules
	HMODULE hMods[1024];
	DWORD cbNeeded;

	//char* importFunctions = (char*)malloc(1024*1024);
	//*importFunctions=0;


	//Walk all modules 
	if( EnumProcessModules(processHandle, hMods, sizeof(hMods), &cbNeeded))
	{
		for (  int i = 0; i < (int)(cbNeeded / sizeof(HMODULE)); i++ )
		{
			HMODULE currentHandle = hMods[i];
			char moduleName[1024];
			GetModuleFileNameA(currentHandle,moduleName,1024);
			//strcat(importFunctions,moduleName);
			//strcat(importFunctions,"\r\n");

			PEFile* peFile2 = new PEFile();
			peFile2->Construct(currentHandle);

			IMAGE_IMPORT_DESCRIPTOR* currentIID = peFile2->GetImportDescriptor();
			if(currentIID != NULL)
			{
				while(currentIID->FirstThunk != 0)
				{
					ImportDescriptorView* currentView = new ImportDescriptorView(peFile2,currentIID);

					currentView->ReplaceImportedFunctions(peFile2,currentIID,apiWapperMethodsMappings);
					//if(address>0)
					//{
					//	DWORD* tem = (DWORD*)address;
					//	*tem = (DWORD)MYMessageBoxW;
					//}
					currentIID++;
				}
			}
		}
	}
}
