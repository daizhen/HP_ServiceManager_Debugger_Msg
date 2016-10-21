#include "StdAfx.h"
#include "ExportTableView.h"

ExportTableView::ExportTableView(PEFile* parent,IMAGE_EXPORT_DIRECTORY* exportDirectory)
{
	this->parent = parent;
	this->exportDirectory = exportDirectory;
	this->name = (char*)((DWORD)parent->GetStartAddr() + exportDirectory->Name);
}

ExportTableView::~ExportTableView(void)
{
}

BOOL ExportTableView::GetFunctionNameByHint(DWORD hint,char* functionName)
{
	DWORD functionCount = exportDirectory->NumberOfFunctions;
	DWORD nameCount = exportDirectory->NumberOfNames;
	DWORD addressOfNames = exportDirectory->NumberOfNames;
	WORD* addressOfOrdinal = (WORD*)((DWORD)parent->GetStartAddr()+exportDirectory->AddressOfNameOrdinals);

	DWORD baseIndex = exportDirectory->Base;
	DWORD functionIdex = -1;
	//hint - baseIndex;
	for(int i=0;i<nameCount;i++)
	{
		if(*(addressOfOrdinal + i) == hint- baseIndex)
		{
			functionIdex = i;
			break;
		}
	}

	if(functionIdex!= -1)
	{
		DWORD* nameAddress = (DWORD*)((DWORD)parent->GetStartAddr()+exportDirectory->AddressOfNames);
		DWORD currentFunctionNameAddress = *(nameAddress + functionIdex);

		char* tem = (char* )((DWORD)parent->GetStartAddr() + currentFunctionNameAddress);
		strcpy(functionName,tem);
		return TRUE;
	}
	return FALSE;

}
