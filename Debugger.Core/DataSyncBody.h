#pragma once
#include <windows.h>
typedef struct _DataSyncBody
{
	DWORD Flag;
	DWORD Length;
	char Content;
} DataSyncBody;