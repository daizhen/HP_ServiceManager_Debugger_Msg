#include "StdAfx.h"
#include "PEFile.h"

PEFile::PEFile(void)
{
}

PEFile::~PEFile(void)
{
	if(startAddr)
	{
		//free(startAddr);
	}
}
DWORD PEFile::GetSectionCount()
{
	if(!peHeader)
	{
		return peHeader->FileHeader.NumberOfSections;
	}

	return -1;
}

void PEFile::Construct(const WCHAR* fileName)
{
	HANDLE fileHandle = (HANDLE)CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL); 
	if(fileHandle != INVALID_HANDLE_VALUE)
	{
		DWORD fileSize = GetFileSize(fileHandle,NULL);
		startAddr = (BYTE*) malloc(fileSize);
		DWORD realSize =-1;
		OVERLAPPED overLapped={0};
		BOOL result = ReadFile(fileHandle,startAddr,fileSize,&realSize,&overLapped);
		dosHeader = (IMAGE_DOS_HEADER*)startAddr;
		peHeader = (IMAGE_NT_HEADERS*)(startAddr + dosHeader->e_lfanew);
		sectionHeader = (IMAGE_SECTION_HEADER*)((DWORD)peHeader + sizeof(IMAGE_NT_HEADERS));
	}

	CloseHandle(fileHandle);
}

void PEFile::Construct(HMODULE hModule)
{
	startAddr = (BYTE*) hModule;
	Construct();
}

void PEFile::Construct()
{
	dosHeader = (IMAGE_DOS_HEADER*)startAddr;
	peHeader = (IMAGE_NT_HEADERS*)(startAddr + dosHeader->e_lfanew);
	sectionHeader = (IMAGE_SECTION_HEADER*)((DWORD)peHeader + sizeof(IMAGE_NT_HEADERS));

	DWORD importDescriptorAddress = peHeader->OptionalHeader.DataDirectory[1].VirtualAddress;
	DWORD exportDirectoryAddress =  peHeader->OptionalHeader.DataDirectory[0].VirtualAddress;
	if(importDescriptorAddress>0)
	{
		importDescriptor = (IMAGE_IMPORT_DESCRIPTOR*)(startAddr + importDescriptorAddress);
	}
	else
	{
		importDescriptor = NULL;
	}

	if(exportDirectoryAddress>0)
	{
		exportDirectory = (IMAGE_EXPORT_DIRECTORY*)(startAddr + exportDirectoryAddress);
	}
	else
	{
		exportDirectory = NULL;
	}
}

BYTE* PEFile::GetStartAddr()
{
	return startAddr;
}
IMAGE_DOS_HEADER* PEFile::GetDosHeader()
{
	return dosHeader;
}
IMAGE_NT_HEADERS* PEFile::GetPeHeader()
{
	return peHeader;
}
IMAGE_SECTION_HEADER* PEFile::GetSectionHeader()
{
	return sectionHeader;
}

IMAGE_IMPORT_DESCRIPTOR* PEFile::GetImportDescriptor()
{
	return importDescriptor;
}

IMAGE_EXPORT_DIRECTORY* PEFile::GetExportDirectory()
{
	return this->exportDirectory;
}