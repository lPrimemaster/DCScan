#include "register.h"
#include "../../version.h"
namespace IO
{
	void createIniFile(LPCTSTR name)
	{
		char path[64];
		char fpath[64];
		GetFullPathName(name, 64, path, NULL);

		WritePrivateProfileString("Version", "major", GET_VERSION_MAJOR().c_str(), path);
		WritePrivateProfileString("Version", "minor", GET_VERSION_MINOR().c_str(), path);

		WritePrivateProfileString("IOLocation", "name", "data_out", path);
		WritePrivateProfileString("IOLocation", "relative_path", "data", path);
		GetFullPathName("data", 64, fpath, NULL);
		WritePrivateProfileString("IOLocation", "absolute_path", fpath, path);
		WritePrivateProfileString("IOLocation", "extension", "csv", path);

		WritePrivateProfileString("UpdateServer", "protocol", "http", path);
		WritePrivateProfileString("UpdateServer", "url", "http://example.com", path);

		WritePrivateProfileString("AcquireSettings", "default_timer", "10000.0", path);
		WritePrivateProfileString("AcquireSettings", "default_channel", "dev1/ai0", path);
		WritePrivateProfileString("AcquireSettings", "default_assigned", "", path);
		WritePrivateProfileString("AcquireSettings", "default_units", "10348", path);
		WritePrivateProfileString("AcquireSettings", "default_min", "-10.0", path);
		WritePrivateProfileString("AcquireSettings", "default_max", "10.0", path);
		WritePrivateProfileString("AcquireSettings", "default_spc", "1000", path);

		WritePrivateProfileString("ControlSettings", "default_delta", "0.001", path);
	}

	IniFileData readIniFile(LPCTSTR name)
	{
		char path[64];
		GetFullPathName(name, 64, path, NULL);
		IniFileProperties properties;

		//64 bytes max string on ini file
		IniFileData data;

		for (auto section : properties.section)
		{
			for (auto sub_sec : properties.sub_sec[section])
			{
				char buffer[64];
				DWORD error = GetPrivateProfileString(section, sub_sec, "Unknown", buffer, 64, path);
				data[section][sub_sec] = buffer;
			}
		}

		//Version specification

		//IO Path location + name + extension

		//Update server protocol + url

		//Acquisition settings

		return data;
	}

	IniFileData readCfgFile(LPCTSTR name)
	{
		char path[64];
		GetFullPathName(name, 64, path, NULL);
		IniFileProperties properties;

		//64 bytes max string on cfg file
		IniFileData data;

		char buffer[64];
		DWORD error;

		error = GetPrivateProfileString("Version", "Major", "Unknown", buffer, 64, path);
		data["Version"]["Major"] = buffer;
		error = GetPrivateProfileString("Version", "Minor", "Unknown", buffer, 64, path);
		data["Version"]["Minor"] = buffer;

		error = GetPrivateProfileString("Version", "Type", "Unknown", buffer, 64, path);
		data["Version"]["Type"] = buffer;

		error = GetPrivateProfileString("Version", "Build", "Unknown", buffer, 64, path);
		data["Version"]["Build"] = buffer;

		return data;
	}
}