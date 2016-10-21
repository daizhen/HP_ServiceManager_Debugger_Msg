#include "StdAfx.h"
#include "HttpUtil.h"

string HttpUtil::ReadLine(const char* rawData, int length,int* nextIndex)
{
	string str;
	while(*nextIndex<length && *(rawData + (*nextIndex))!='\r')
	{
		str.push_back(*(rawData + (*nextIndex)));
		(*nextIndex)++;
	}
	if(*nextIndex < length)
	{
		char nextChar = *(rawData + ((*nextIndex) + 1));
		if(nextChar == '\n')
		{
			(*nextIndex)+=2;
		}
	}

	return str;
}

void HttpUtil::ConstructHeaderItems(const char* rawData, int length,map<string,string>* items)
{
	int nextIndex = 0;
	while(nextIndex<length - 1)
	{
		string lineData = ReadLine(rawData,length,&nextIndex);
		int index =  lineData.find(":");
		string key = lineData.substr(0,index);
		string value =  lineData.substr(index+1);
		if(items->find(key) == items->end())
		{
			items->insert(make_pair(key,value));
		}
		else
		{
			items->find(key)->second =value;
		}
	}
}

HttpUtil::HttpUtil(void)
{
}

HttpUtil::~HttpUtil(void)
{
}
