#pragma once
#include <map>
#include <string>

using namespace std;

class HttpUtil
{
public:
	static string ReadLine(const char*, int length,int* nextIndex);
	static void ConstructHeaderItems(const char*, int length,map<string,string>* items);
	//static string getString(const char*, int length);
	HttpUtil(void);
	~HttpUtil(void);
};
