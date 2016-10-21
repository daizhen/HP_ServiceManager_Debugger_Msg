#include <windows.h>
#include <stdio.h>
void Inject(HANDLE hProcess, const char* dllname, const char* funcname);