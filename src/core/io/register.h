#pragma once
#include <vector>
#include <map>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif


namespace IO
{
	typedef std::map<const char*, std::map<const char*, std::string>> IniFileData;

	struct IniFileProperties
	{
		const std::vector<const char*> section = { "Version", "IOLocation", "UpdateServer", "AcquireSettings", "ControlSettings" };
		const std::map<const char*, std::vector<const char*>> sub_sec =
		{
			{ "Version",			{ "major", "minor" } },											//Version
			{ "IOLocation",			{ "name", "relative_path", "absolute_path", "extension" } },	//IOLocation
			{ "UpdateServer",		{ "protocol", "url" } },										//UpdateServer
			{ "AcquireSettings",	{	"default_timer",
										"default_channel",
										"default_assigned",
										"default_units",
										"default_min",
										"default_max",
										"default_spc"
									} 
			},																						//AcquireSettings
			{ "ControlSettings",	{ "default_delta" } }											//ControlSettings
		};
	};

	void createIniFile(LPCTSTR name);
	IniFileData readIniFile(LPCTSTR name);
	IniFileData readCfgFile(LPCTSTR name);
}
