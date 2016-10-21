#pragma once
#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>
#include "Wininet.h"
typedef FARPROC (CALLBACK *SYS_GetProcAddress)(HMODULE,LPCSTR); 

extern SYS_GetProcAddress originalGetProcAddress;
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
    );
int
WINAPI
MYMessageBoxW(
    __in_opt HWND hWnd,
    __in_opt LPCWSTR lpText,
    __in_opt LPCWSTR lpCaption,
    __in UINT uType);

WINUSERAPI
LRESULT
WINAPI
SendMessageW_NEW(
    __in HWND hWnd,
    __in UINT Msg,
    __in WPARAM wParam,
    __in LPARAM lParam);

BOOL WINAPI HttpSendRequestW_NEW(
    __in HINTERNET hRequest,
    __in_ecount_opt(dwHeadersLength) LPCWSTR lpszHeaders,
    __in DWORD dwHeadersLength,
    __in_bcount_opt(dwOptionalLength) LPVOID lpOptional,
    __in DWORD dwOptionalLength
    );
HINTERNET InternetConnectW_NEW(
    __in HINTERNET hInternet,
    __in LPCWSTR lpszServerName,
    __in INTERNET_PORT nServerPort,
    __in_opt LPCWSTR lpszUserName,
    __in_opt LPCWSTR lpszPassword,
    __in DWORD dwService,
    __in DWORD dwFlags,
    __in_opt DWORD_PTR dwContext
    );

FARPROC
WINAPI
GetProcAddress_NEW (
    __in HMODULE hModule,
    __in LPCSTR lpProcName
    );
int
WSAAPI
WSASendDisconnect_NEW(
    IN SOCKET s,
    IN LPWSABUF lpOutboundDisconnectData
    );
int
WSAAPI
recv_NEW(
    IN SOCKET s,
    __out_bcount_part(len, return) __out_data_source(NETWORK) char FAR * buf,
    IN int len,
    IN int flags
    );
int
WSAAPI
send_NEW(
    IN SOCKET s,
    __in_bcount(len) const char FAR * buf,
    IN int len,
    IN int flags
    );
int
WSAAPI
connect_NEW(
    IN SOCKET s,
    __in_bcount(namelen) const struct sockaddr FAR * name,
    IN int namelen
    );


int WSAAPI closesocket_NEW ( IN SOCKET s);