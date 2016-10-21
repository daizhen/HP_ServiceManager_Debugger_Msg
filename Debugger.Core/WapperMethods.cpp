#include "StdAfx.h"
#include "WapperMethods.h"
#include "DataSync.h"
#include "HttpRequestHeader.h"
#include <string>
#include <WINSOCK2.H> 
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
extern DataSync* dataSync;
extern HANDLE semaphoreEmpty;
extern HANDLE semaphoreFull;
bool bodyExpected = false;
int bodyLength = -1;
int headerLength = -1;
int
WSAAPI
WSASend_NEW(
IN SOCKET s,
__in_ecount(dwBufferCount) LPWSABUF lpBuffers,
IN DWORD dwBufferCount,
__out_opt LPDWORD lpNumberOfBytesSent,
IN DWORD dwFlags,
__in_opt LPWSAOVERLAPPED lpOverlapped,
__in_opt LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
)
{
	typedef int (CALLBACK *WSASend_SYS)(SOCKET, LPWSABUF, DWORD, LPDWORD, DWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
	HMODULE ws2_32 = GetModuleHandle(TEXT("WS2_32.dll"));
	WSASend_SYS sendMethod = (WSASend_SYS)GetProcAddress(ws2_32, "WSASend");
	return (*sendMethod)(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
}

int
WINAPI
MYMessageBoxW(
__in_opt HWND hWnd,
__in_opt LPCWSTR lpText,
__in_opt LPCWSTR lpCaption,
__in UINT uType)
{
	typedef int (CALLBACK *SysMessage)(HWND, LPCWSTR, LPCWSTR, UINT);
	HMODULE user32 = GetModuleHandle(TEXT("User32.dll"));
	SysMessage messageMethod = (SysMessage)GetProcAddress(user32, "MessageBoxW");
	return (*messageMethod)(hWnd, lpText, lpCaption, uType);
}

LRESULT
WINAPI
SendMessageW_NEW(
__in HWND hWnd,
__in UINT Msg,
__in WPARAM wParam,
__in LPARAM lParam)
{
	typedef LRESULT(CALLBACK *SysMessage)(HWND, UINT, WPARAM, LPARAM);
	HMODULE user32 = GetModuleHandle(TEXT("User32.dll"));
	SysMessage messageMethod = (SysMessage)GetProcAddress(user32, "SendMessageW");
	return (*messageMethod)(hWnd, Msg, wParam, lParam);
}

BOOL WINAPI HttpSendRequestW_NEW(
	__in HINTERNET hRequest,
	__in_ecount_opt(dwHeadersLength) LPCWSTR lpszHeaders,
	__in DWORD dwHeadersLength,
	__in_bcount_opt(dwOptionalLength) LPVOID lpOptional,
	__in DWORD dwOptionalLength
	)
{
	typedef BOOL(CALLBACK *SysMessage)(HINTERNET, LPCWSTR, DWORD, LPVOID, DWORD);
	HMODULE winInet = GetModuleHandle(TEXT("WININET.dll"));
	SysMessage messageMethod = (SysMessage)GetProcAddress(winInet, "HttpSendRequestW");
	return (*messageMethod)(hRequest, lpszHeaders, dwHeadersLength, lpOptional, dwOptionalLength);
}

HINTERNET InternetConnectW_NEW(
	__in HINTERNET hInternet,
	__in LPCWSTR lpszServerName,
	__in INTERNET_PORT nServerPort,
	__in_opt LPCWSTR lpszUserName,
	__in_opt LPCWSTR lpszPassword,
	__in DWORD dwService,
	__in DWORD dwFlags,
	__in_opt DWORD_PTR dwContext
	)
{
	typedef HINTERNET(CALLBACK *SysMessage)(HINTERNET, LPCWSTR, INTERNET_PORT, LPCWSTR, LPCWSTR, DWORD, DWORD, DWORD_PTR);
	HMODULE winInet = GetModuleHandle(TEXT("WININET.dll"));
	SysMessage messageMethod = (SysMessage)GetProcAddress(winInet, "InternetConnectW");
	return (*messageMethod)(hInternet, lpszServerName, nServerPort, lpszUserName, lpszPassword, dwService, dwFlags, dwContext);
}
FARPROC
WINAPI
GetProcAddress_NEW(
__in HMODULE hModule,
__in LPCSTR lpProcName
)
{
	return (*originalGetProcAddress)(hModule, lpProcName);
	//typedef FARPROC (CALLBACK *SYS)(hModule,lpProcName); 
	//HMODULE winInet = GetModuleHandle(TEXT("KERNEL32.dll"));
	//SysMessage messageMethod=(SysMessage)GetProcAddress(winInet,"GetProcAddress"); 
	//return (*messageMethod)(hInternet,lpszServerName,nServerPort,lpszUserName,lpszPassword,dwService,dwFlags,dwContext);
}

int
WSAAPI
WSASendDisconnect_NEW(
IN SOCKET s,
IN LPWSABUF lpOutboundDisconnectData
)
{
	if (dataSync->GetHeader()->MonitoredSocket == s)
	{
		//Clear the socket field
		dataSync->GetHeader()->MonitoredSocket = 0;
		//dataSync->GetHeader()->Enable = 0;
	}
	typedef int (WSAAPI *SysMessage)(SOCKET, LPWSABUF);
	HMODULE winInet = GetModuleHandle(TEXT("WS2_32.dll"));
	SysMessage messageMethod = (SysMessage)GetProcAddress(winInet, "WSASendDisconnect");
	return (*messageMethod)(s, lpOutboundDisconnectData);
}

int WSAAPI closesocket_NEW(IN SOCKET s)
{
	if (dataSync->GetHeader()->MonitoredSocket == s)
	{
		//Clear the socket field
		dataSync->GetHeader()->MonitoredSocket = 0;
		//dataSync->GetHeader()->Enable = 0;
	}

	typedef int (WSAAPI *Sysrecv)(SOCKET);
	HMODULE WS2_32 = GetModuleHandle(TEXT("WS2_32.dll"));
	Sysrecv sysMethod = (Sysrecv)GetProcAddress(WS2_32, "closesocket");
	int returnValue = (*sysMethod)(s);
	return returnValue;
}

int
WSAAPI
recv_NEW(
IN SOCKET s,
__out_bcount_part(len, return) __out_data_source(NETWORK) char FAR * buf,
IN int len,
IN int flags
)
{
	int returnValue = 0;
	if (s == dataSync->GetHeader()->MonitoredSocket &&dataSync->NeedReadDataFromDebugger())
	{
		returnValue = dataSync->ReadDataFromDebugger((LPVOID)buf, len);
		if (!dataSync->NeedReadDataFromDebugger())
		{
			ReleaseSemaphore(semaphoreEmpty, 1, NULL);
		}
	}
	else
	{
		typedef int (WSAAPI *Sysrecv)(SOCKET, char FAR *, int, int);
		HMODULE WS2_32 = GetModuleHandle(TEXT("WS2_32.dll"));
		Sysrecv sysMethod = (Sysrecv)GetProcAddress(WS2_32, "recv");
		returnValue = (*sysMethod)(s, buf, len, flags);

		if (dataSync->GetHeader()->Enable && s == dataSync->GetHeader()->MonitoredSocket)
		{
			if (returnValue > 0)
			{
				WaitForSingleObject(semaphoreEmpty, INFINITE);
				dataSync->WriteNetData((LPVOID)buf, returnValue);
				ReleaseSemaphore(semaphoreFull, 1, NULL);


				WaitForSingleObject(semaphoreEmpty, INFINITE);

				//Process aditional steps here.
				if (dataSync->NeedReadDataFromDebugger())
				{
					returnValue = dataSync->ReadDataFromDebugger((LPVOID)buf, len);
					char* tem = (char*)buf;
				}
				if (!dataSync->NeedReadDataFromDebugger())
				{
					ReleaseSemaphore(semaphoreEmpty, 1, NULL);
				}
			}
		}
	}
	return returnValue;
}

void ResetVars()
{
	bodyExpected = false;
	bodyLength = -1;
	headerLength = -1;
}
int
WSAAPI
send_NEW(
IN SOCKET s,
__in_bcount(len) const char FAR * buf,
IN int len,
IN int flags
)
{
	typedef int (WSAAPI *Syssend)(SOCKET, const char FAR *, int, int);
	HMODULE WS2_32 = GetModuleHandle(TEXT("WS2_32.dll"));
	Syssend sysMethod = (Syssend)GetProcAddress(WS2_32, "send");
	if (s == dataSync->GetHeader()->MonitoredSocket ||
		dataSync->GetHeader()->MonitoredSocket == 0)
	{
		if (bodyExpected)
		{
			//int* dataLength = (int*)(dataSync->GetStartAddress() + 420);
			dataSync->AppendNetData((LPVOID)buf, len);
			bodyLength -= len;
			if (bodyLength == 0)
			{
				bodyExpected = false;
			}
		}
		else
		{
			HttpRequestHeader header = HttpRequestHeader(buf, len);
			if (header.IsValid())
			{
				map<string, string> items = *header.GetItems();
				map<string, string>::iterator cookieIterator = header.GetItems()->find(string("Cookie"));
				map<string, string>::iterator authorizationIterator = header.GetItems()->find(string("Authorization"));
				if (cookieIterator != header.GetItems()->end() && authorizationIterator != header.GetItems()->end())
				{
					dataSync->WriteCookie(cookieIterator->second.c_str(), cookieIterator->second.length());
					dataSync->WriteAuthorizationString(authorizationIterator->second.c_str(), authorizationIterator->second.length());
					dataSync->GetHeader()->MonitoredSocket = s;
				}

				map<string, string>::iterator contentLengthIterator = header.GetItems()->find(string("Content-Length"));
				if (contentLengthIterator != header.GetItems()->end())
				{
					string strLength = contentLengthIterator->second;
					bodyLength = atoi(strLength.c_str());
					bodyExpected = true;
				}
			}
			//dataSync->WriteNetData((LPVOID)buf,len);
		}
	}
	int returnValue = (*sysMethod)(s, buf, len, flags);
	return returnValue;
}

int
WSAAPI
connect_NEW(
IN SOCKET s,
__in_bcount(namelen) const struct sockaddr FAR * name,
IN int namelen
)
{
	if (dataSync->GetHeader()->MonitoredSocket == 0 || dataSync->GetHeader()->Port == 0 || dataSync->GetHeader()->IPAddress[0] == '\0')
	{
		if (name->sa_family == AF_INET6)
		{
			const SOCKADDR_IN6* socketIn = (SOCKADDR_IN6*)name;
			int remotePort = ntohs(socketIn->sin6_port);
			dataSync->WritePort(remotePort);
			dataSync->GetHeader()->MonitoredSocket = s;
			int value = FILE_MAP_ALL_ACCESS;
			//char *ip = inet_ntoa(socketIn->sin6_addr);
			//long ipAddress = *(long*)(socketIn->sin6_addr);
			dataSync->WriteIPAddress(socketIn->sin6_addr.u.Byte + 10);
		}
		else
		{
			const SOCKADDR_IN* socketIn = (SOCKADDR_IN*)name;
			int remotePort = ntohs(socketIn->sin_port);
			dataSync->WritePort(remotePort);
			dataSync->GetHeader()->MonitoredSocket = s;
			int value = FILE_MAP_ALL_ACCESS;
			char *ip = inet_ntoa(socketIn->sin_addr);
			//long ipAddress = *(long*)name->sa_data;
			dataSync->WriteIPAddress(name->sa_data);
		}
	}

	HANDLE fileMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, TEXT("DataSync"));

	typedef int (WSAAPI *Sysconnect)(SOCKET, const struct sockaddr FAR *, int);
	HMODULE WS2_32 = GetModuleHandle(TEXT("WS2_32.dll"));
	Sysconnect sysMethod = (Sysconnect)GetProcAddress(WS2_32, "connect");
	return (*sysMethod)(s, name, namelen);
}