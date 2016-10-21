#pragma once
#include <windows.h>
typedef struct _DataSyncHeader
{
	char IPAddress[8];
	DWORD Port;
	DWORD CookieLength;
	char Cookie[200];
	DWORD AuthLength;
	char Authorization[200];
	DWORD MonitoredSocket;
	DWORD Enable;
} DataSyncHeader;