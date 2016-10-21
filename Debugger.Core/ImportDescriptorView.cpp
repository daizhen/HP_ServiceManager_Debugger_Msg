#include "StdAfx.h"
#include "ImportDescriptorView.h"
#include "ExportTableView.h"

ImportDescriptorView::ImportDescriptorView(PEFile* parent,IMAGE_IMPORT_DESCRIPTOR* descriptor)
{
	this->parent = parent;
	this->descriptor = descriptor;
	name =(char*)((DWORD)parent->GetStartAddr() + descriptor->Name);
	importNameTable = (IMAGE_THUNK_DATA*)((DWORD)parent->GetStartAddr() + descriptor->OriginalFirstThunk);
	importAddressTable = (IMAGE_THUNK_DATA*)((DWORD)parent->GetStartAddr() + descriptor->FirstThunk);
}

DWORD ImportDescriptorView::GetAddressOfFunction(const char* functionName)
{
	if(descriptor == NULL)
	{
		return 0;
	}
	IMAGE_THUNK_DATA* currentINT = importNameTable;
	int index = 0;
	while(currentINT!=NULL && currentINT->u1.AddressOfData!=0)
	{
		IMAGE_IMPORT_BY_NAME* inportByName = (IMAGE_IMPORT_BY_NAME*)((DWORD)parent->GetStartAddr() + currentINT->u1.AddressOfData);
		char* currentFunctionName = (char*)inportByName->Name;

		//The function found
		if(strcmp(functionName,currentFunctionName)==0)
		{
			break;
		}
		currentINT++;
		index++;
	}
	if(currentINT->u1.AddressOfData!=0)
	{
		IMAGE_THUNK_DATA* destIAT = importAddressTable + index;
		return (DWORD)destIAT;
	}
	return 0;
}

void ImportDescriptorView::GetAllImportFunctions(PEFile* parent,IMAGE_IMPORT_DESCRIPTOR* descriptor,char* resultString)
{
	if(descriptor == NULL)
	{
		return;
	}
	
	char* importModuleName = (char*)((DWORD)parent->GetStartAddr() + descriptor->Name);
	strcat(resultString,"\t");
	strcat(resultString,this->name);
	strcat(resultString,"\r\n");

	IMAGE_THUNK_DATA* currentINT = importNameTable;

	//Loop All the import functions.
	while(currentINT->u1.AddressOfData!=0)
	{
		if((currentINT->u1.AddressOfData & 0x80000000) == 0)
		{
			IMAGE_IMPORT_BY_NAME* inportByName = (IMAGE_IMPORT_BY_NAME*)((DWORD)parent->GetStartAddr() + currentINT->u1.AddressOfData);
			char* currentFunctionName = (char*)inportByName->Name;
			strcat(resultString,"\t\t");
			strcat(resultString,currentFunctionName);
			strcat(resultString,"\r\n");
		}
		else
		{
			DWORD importHint = (currentINT->u1.AddressOfData & 0xFFFF);
			HMODULE  exportModule =  GetModuleHandleA(importModuleName);
			PEFile exportPE;
			exportPE.Construct(exportModule);
			ExportTableView  exportTable(&exportPE,exportPE.GetExportDirectory());
			char currentFunctionNameTem[1024];
			
			//Get function from hint value.
			if(exportTable.GetFunctionNameByHint(importHint,currentFunctionNameTem))
			{
				strcat(resultString,"\t\t");
				strcat(resultString,currentFunctionNameTem);
				strcat(resultString,"\t");
				strcat(resultString,"hint");
				strcat(resultString,"\r\n");
			}
			else
			{
				
			}
		}
		currentINT++;
	}
}

/*

*/
bool enableDebugPriv()
{
     HANDLE hToken;
     LUID sedebugnameValue;
     TOKEN_PRIVILEGES tkp;
  
     if (!OpenProcessToken(GetCurrentProcess(), 
         TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
	 {
         return false;
     }

     if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue)) 
	 {
         CloseHandle(hToken);
         return false;
     }

     tkp.PrivilegeCount = 1;
     tkp.Privileges[0].Luid = sedebugnameValue;
     tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

     if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL)) 
	 {
         CloseHandle(hToken);
         return false;
     }

     return true;
}

void ImportDescriptorView::ReplaceImportedFunctions(PEFile* parent,IMAGE_IMPORT_DESCRIPTOR* descriptor,map<string,unsigned int> apiMappings)
{
	if(descriptor == NULL)
	{
		return;
	}

	char* importModuleName = (char*)((DWORD)parent->GetStartAddr() + descriptor->Name);

	IMAGE_THUNK_DATA* currentINT = importNameTable;

	int methodIndex = 0;
	//Loop All the import functions.
	while(currentINT->u1.AddressOfData!=0)
	{
		char* currentFunctionName;
		if((currentINT->u1.AddressOfData & 0x80000000) == 0)
		{
			IMAGE_IMPORT_BY_NAME* inportByName = (IMAGE_IMPORT_BY_NAME*)((DWORD)parent->GetStartAddr() + currentINT->u1.AddressOfData);
			currentFunctionName = (char*)inportByName->Name;
		}
		else
		{
			DWORD importHint = (currentINT->u1.AddressOfData & 0xFFFF);
			HMODULE  exportModule =  GetModuleHandleA(importModuleName);
			PEFile exportPE;
			exportPE.Construct(exportModule);
			ExportTableView  exportTable(&exportPE,exportPE.GetExportDirectory());
			char currentFunctionNameTem[1024];
			//Get function from hint value.
			if(exportTable.GetFunctionNameByHint(importHint,currentFunctionNameTem))
			{
				currentFunctionName = currentFunctionNameTem;
			}
			else
			{
				currentFunctionName = NULL;
			}
		}
		if(currentFunctionName)
		{
			map<string,unsigned int>::iterator iteratorMethodName = apiMappings.find(currentFunctionName);
			if(iteratorMethodName != apiMappings.end())
			{
				IMAGE_THUNK_DATA* destIAT = importAddressTable + methodIndex;
				bool isDebug = enableDebugPriv();

				//DWORD* methodAddress = (DWORD*)(destIAT);
				DWORD methodAddress = (DWORD)(destIAT->u1.AddressOfData);
				int newAddress = iteratorMethodName->second;
				HANDLE currentProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
				SIZE_T realSize;

				DWORD dwOldProter;
				BOOL protectUpdate = VirtualProtect(destIAT,4,PAGE_WRITECOPY,&dwOldProter);

				BOOL result = WriteProcessMemory(currentProcess,(LPVOID)destIAT,(LPCVOID)(&newAddress),4,&realSize);
				//typedef int (CALLBACK *SysMessage)(HWND,LPCWSTR,LPCWSTR,UINT); 
				//HMODULE user32 = GetModuleHandle(TEXT("User32.dll"));
				//SysMessage messageMethod=(SysMessage)GetProcAddress(user32,"MessageBoxW"); 
				//&methodAddress = iteratorMethodName->second;
				int a=0;
				a++;
			}
		}

		currentINT++;
		methodIndex++;
	}
}
ImportDescriptorView::~ImportDescriptorView(void)
{
}
