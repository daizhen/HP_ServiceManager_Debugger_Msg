#pragma once

#include "vector"
using namespace std;

class ProcessHelper
{
public:
	ProcessHelper(void);
	~ProcessHelper(void);
public:
	vector<PROCESSENTRY32> GetProcessList();
	vector<THREADENTRY32> GetThreadList(DWORD processId);
	DWORD GetWindowThreadId(DWORD processId);

};
