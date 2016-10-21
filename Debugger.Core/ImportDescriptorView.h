#pragma once
#include "PEFile.h"
#include <map>
#include <string>
using namespace std;

class ImportDescriptorView
{
private:
	PEFile* parent;
	char* name;
	IMAGE_IMPORT_DESCRIPTOR* descriptor;
	IMAGE_THUNK_DATA* importNameTable;
	IMAGE_THUNK_DATA* importAddressTable;

public:
	ImportDescriptorView(PEFile* parent,IMAGE_IMPORT_DESCRIPTOR* descriptor);
	void GetAllImportFunctions(PEFile* parent,IMAGE_IMPORT_DESCRIPTOR* descriptor,char* resultStrings);
	DWORD GetAddressOfFunction(const char* functionName);
	void ReplaceImportedFunctions(PEFile* parent,IMAGE_IMPORT_DESCRIPTOR* descriptor,map<string,unsigned int>);
	~ImportDescriptorView(void);
};
