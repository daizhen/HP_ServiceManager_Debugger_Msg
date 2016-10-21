#pragma once

#include "stdafx.h"
class PEFile
{
private :
	BYTE* startAddr;
	IMAGE_DOS_HEADER* dosHeader;
	IMAGE_NT_HEADERS* peHeader;
	IMAGE_SECTION_HEADER* sectionHeader;
	IMAGE_IMPORT_DESCRIPTOR* importDescriptor;
	IMAGE_EXPORT_DIRECTORY* exportDirectory;
	void Construct();
public:
	PEFile(void);
	~PEFile(void);

	BYTE* GetStartAddr();
    IMAGE_DOS_HEADER* GetDosHeader();
	IMAGE_NT_HEADERS* GetPeHeader();
	IMAGE_SECTION_HEADER* GetSectionHeader();

	IMAGE_IMPORT_DESCRIPTOR* GetImportDescriptor();
	IMAGE_EXPORT_DIRECTORY* GetExportDirectory();
	void Construct(const WCHAR* fileName);
	void Construct(HMODULE hModule);
	DWORD GetSectionCount();
};
