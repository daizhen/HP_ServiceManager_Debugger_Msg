#include "StdAfx.h"
#include "HttpRequestHeader.h"
#include "HttpUtil.h"

HttpRequestHeader::HttpRequestHeader(const char* rawData, int length )
{
	//
	if(length < 2)
	{
		valid = false;
	}
	if(*(rawData+length-2) != '\r' || *(rawData+length-1) != '\n' )
	{
		valid = false;
	}

	int currentIndex = 0;

	if(valid)
	{
		//Post uri version
		string firstLine = HttpUtil::ReadLine(rawData,length,&currentIndex);

		HttpUtil::ConstructHeaderItems(rawData+currentIndex,length - currentIndex,&items);
	}
}

map<string,string>* HttpRequestHeader::GetItems()
{
	return &items;
}

bool HttpRequestHeader::IsValid()
{
	return valid;
}

HttpRequestHeader::~HttpRequestHeader(void)
{
}
