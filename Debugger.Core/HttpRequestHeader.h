#pragma once
#include <string>
#include <map>
using namespace std;
class HttpRequestHeader
{
private :
	bool valid;
	string method;
	string uri;
	string  version;
	map<string,string> items;
public:
	bool IsValid();
	string GetMethod();
	string GetUri();
	string GetVersion();
	map<string,string>* GetItems();
	void SetValid(bool);
	HttpRequestHeader(const char*, int length);
	~HttpRequestHeader(void);
};
