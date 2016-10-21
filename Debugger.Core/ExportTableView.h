#pragma once
#include "PEFile.h"
#include <map>
#include <string>
class ExportTableView
{
private:
	PEFile* parent;
	char* name;
	IMAGE_EXPORT_DIRECTORY* exportDirectory;
public:
	ExportTableView(PEFile* parent,IMAGE_EXPORT_DIRECTORY* exportDirectory);
	BOOL GetFunctionNameByHint(DWORD hint,char* functionName);
	~ExportTableView(void);
};
