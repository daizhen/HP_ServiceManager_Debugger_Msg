#pragma once
#include <windows.h>
#include "DataSyncHeader.h"
#include "DataSyncBody.h"

class DataSync
{
private:
	int fileMappingSize;
	HANDLE hFile;
	HANDLE hFileMapping;
	HANDLE hBackFileMapping;
	LPVOID memoryStartAddress;
	LPVOID memoryBackStartAddress;
public:
	DataSyncHeader* GetHeader();
	DataSyncBody* GetBody();
	void WriteData(DWORD offset, LPVOID data, int length);
	void WriteIPAddress(const char* ipAddress);
	void WriteIPAddress(const UCHAR* ipAddress);
	void WritePort(DWORD port);
	void WriteCookie(const char* cookie,DWORD length);
	void WriteAuthorizationString(const char* authorizationStr,DWORD length);
	void WriteNetData(LPVOID data,DWORD length);
	void AppendNetData(LPVOID data,DWORD length);
	DWORD ReadDataFromDebugger(LPVOID outData,DWORD length);
	BOOL NeedKeepData();
	BOOL NeedReadDataFromDebugger();
	LPVOID GetStartAddress();

	DataSync();
	~DataSync();
};