#pragma once
#include <map>
#include <string>
using namespace std;

class ImportMethodsUpdater
{
private:
	map<string,unsigned int> apiWapperMethodsMappings;
public:
	ImportMethodsUpdater(void);
	~ImportMethodsUpdater(void);
	map<string,unsigned int>* getMappings()
	{
		return &apiWapperMethodsMappings;
	}
	void updateImportAPIs();
	void getAllImportMethods();
};
