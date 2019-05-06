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
		// Add the lines corresponding to the control values
		std::vector<const char*> section = { "Version", "IOLocation", "UpdateServer", "AcquireSettings", "ControlSettings" };
		std::map<const char*, std::vector<const char*>> sub_sec =
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

	template<typename T>
	inline std::vector<T> convertBracketValue(const std::string& str);

	template<>
	inline std::vector<float> convertBracketValue(const std::string& str)
	{
		std::string::const_iterator it = str.begin();
		if (*it++ != '{')
		{
			return std::vector<float>();
		}
		else
		{
			while (*it != '}')
			{
				while (*it == ' ') it++;
				//Break in substrings with commas, also, dispose of the brackets
			}
		}
	}

	template<>
	inline std::vector<int> convertBracketValue(const std::string& str)
	{

	}
}
