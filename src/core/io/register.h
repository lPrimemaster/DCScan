#pragma once
#include <vector>
#include <map>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif

typedef std::map<const char*, std::map<const char*, std::string>> IniFileData;

struct IniFileProperties
{
	std::vector<const char*> section = { "VERSION", "IOLOC", "UPDATESRV", "ACQSETTINGS", "CTRLSETTINGS" };
	std::map<const char*, std::vector<const char*>> sub_sec =
	{
		{ "VERSION",		{ "MAJOR", "MINOR" } },											//VERSION
		{ "IOLOC",			{ "NAME", "RELATIVE_PATH", "ABSOLUTE_PATH", "EXTENSION" } },	//IOLOC
		{ "UPDATESRV",		{ "PROTOCOL", "URL" } },										//UPDATESERV
		{ "ACQSETTINGS",	{ "DEFAULT_TIMER" } },											//ACQSETTINGSS
		{ "CTRLSETTINGS",	{ "DEFAULT_DELTA" } }											//CTRLSETTINGS
	};
};

void createIniFile(LPCTSTR name);
IniFileData readIniFile(LPCTSTR name);
