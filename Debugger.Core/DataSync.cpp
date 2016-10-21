#include "StdAfx.h"
#include "DataSync.h"

DataSync::DataSync(void)
{
	this->fileMappingSize = 1024*1024*4;	
	//hFile = CreateFileA("DataSync.zip",GENERIC_WRITE | GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_FLAG_SEQUENTIAL_SCAN,NULL);
	hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,fileMappingSize,TEXT("DataSync"));
	memoryStartAddress = MapViewOfFile(hFileMapping,FILE_MAP_ALL_ACCESS,0,0,fileMappingSize);

	//Create another file mapping, which will store data from debugger.
	hBackFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,fileMappingSize,TEXT("DataSyncBack"));
	memoryBackStartAddress = MapViewOfFile(hBackFileMapping,FILE_MAP_ALL_ACCESS,0,0,fileMappingSize);
}

DataSync::~DataSync(void)
{
}

DataSyncHeader* DataSync::GetHeader()
{
	return (DataSyncHeader*) memoryStartAddress;
}
DataSyncBody* DataSync::GetBody()
{
	int size =  sizeof(DataSyncHeader);
	return (DataSyncBody*) ((DWORD)memoryStartAddress + sizeof(DataSyncHeader));
}

void DataSync::WriteData(DWORD offset,LPVOID data, int length)
{
	//memcpy(memoryStartAddress,(LPVOID)&length,sizeof(int));
	DWORD dataAddress = (DWORD)memoryStartAddress + offset;
	memcpy((LPVOID)dataAddress,data,length);
}
void DataSync::WriteIPAddress(const char* ipAddress)
{
	DWORD ipLength = sizeof(GetHeader()->IPAddress)/(sizeof(GetHeader()->IPAddress[0]));
	memcpy((LPVOID)&(GetHeader()->IPAddress),ipAddress,ipLength);
}
void DataSync::WriteIPAddress(const UCHAR* ipAddress)
{
	DWORD ipLength = sizeof(GetHeader()->IPAddress) / (sizeof(GetHeader()->IPAddress[0]));
	memcpy((LPVOID)&(GetHeader()->IPAddress), ipAddress, ipLength);
}
void DataSync::WritePort(DWORD port)
{
	//DWORD offset = 8;
	//WriteData(offset,(LPVOID)(&port),4);
	GetHeader()->Port = port;
}
void DataSync::WriteCookie(const char* cookie,DWORD length)
{
	//DWORD offset = 12;
	//WriteData(offset,(LPVOID)(&length),4);
	//offset+= 4;
	//WriteData(offset,(LPVOID)(cookie),length);
	GetHeader()->CookieLength = length;
	memcpy((LPVOID)&(GetHeader()->Cookie),cookie,length);
}
void DataSync::WriteAuthorizationString(const char* authorizationStr,DWORD length)
{
	//DWORD offset = 216;//12+4+200;
	//WriteData(offset,(LPVOID)(&length),4);
	//offset+= 4;
	//WriteData(offset,(LPVOID)(authorizationStr),length);
	GetHeader()->AuthLength = length;
	memcpy((LPVOID)&(GetHeader()->Authorization),authorizationStr,length);
}
void DataSync::WriteNetData(LPVOID data,DWORD length)
{
	char* cData = (char*)data;
	char* httpPrefix = "HTTP/1.1";
	int prefixLength = strlen(httpPrefix);

	bool needKeepData = false;

	if(strlen(cData)>prefixLength)
	{	
		for(int i=0;i<prefixLength;i++)
		{
			if(*(cData + i)!=*(httpPrefix+ i))
			{
				needKeepData = true;
				break;
			}
		}
	}
	else
	{
		if(NeedKeepData())
		{
			needKeepData = true;
		}
		else
		{
			needKeepData = false;
		}
	}

	if(!needKeepData)
	{
		//DWORD offset = 424;//216+4+4+200;
		//WriteData(offset,(LPVOID)(&length),4);
		//offset+= 4;
		//WriteData(offset,data,length);
		GetBody()->Length = length;
		memcpy((LPVOID)&(GetBody()->Content),data,length);

	}
	else
	{
		AppendNetData(data,length);
	}
}
void DataSync::AppendNetData(LPVOID data,DWORD length)
{
	//DWORD offset = 424;//216+4+4+200;

	//int* dataLength =  (int*)((DWORD)memoryStartAddress + offset);
	////WriteData(offset,(LPVOID)(&length),4);
	//offset = offset+4+(*dataLength);
	//*dataLength +=length;
	//WriteData(offset,data,length);
	DWORD  contentBaseAddress = (DWORD)&GetBody()->Content;
	DWORD existingLength = GetBody()->Length;
	memcpy((LPVOID)(contentBaseAddress + existingLength),data,length);
	GetBody()->Length += length;
}

BOOL DataSync::NeedKeepData()
{
	//DWORD offset = 420;//216+4+4+200;
	//int* flag =  (int*)((DWORD)memoryStartAddress + offset);
	//return (*flag);
	return GetBody()->Flag;
}
LPVOID DataSync::GetStartAddress()
{
	return memoryStartAddress;
}

DWORD DataSync::ReadDataFromDebugger(LPVOID outData, DWORD length)
{
	DWORD* dataLength = (DWORD*)memoryBackStartAddress;
	DWORD dataLengthCopy = * dataLength;
	DWORD* nextIndex = (DWORD*)((DWORD)memoryBackStartAddress+4);

	if(*dataLength>0)
	{
		if(*dataLength > length)
		{
			memcpy(outData,(LPVOID)((DWORD)memoryBackStartAddress+8+(*nextIndex)),length);
			*dataLength = *dataLength - length;
			if(*dataLength ==0)
			{
				*nextIndex = 0;
			}
			else
			{
				*nextIndex = *nextIndex + length;
			}
			
			dataLengthCopy = length;
		}
		else
		{
			memcpy(outData,(LPVOID)((DWORD)memoryBackStartAddress+8+(*nextIndex)),*dataLength);
			//Set the length to 0 to avoid re-read the data next time.
			*dataLength =0;
			*nextIndex=0;
		}
	}

	return dataLengthCopy;
}

BOOL DataSync::NeedReadDataFromDebugger()
{
	DWORD* dataLength = (DWORD*)memoryBackStartAddress;
	if(*dataLength ==0)
	{
		return FALSE;
	}
	return TRUE;
}