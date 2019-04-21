#include "register.h"
namespace IO
{
	void createIniFile(LPCTSTR name)
	{
		char path[64];
		char fpath[64];
		GetFullPathName(name, 64, path, NULL);

		WritePrivateProfileString("VERSION", "MAJOR", "2", path);
		WritePrivateProfileString("VERSION", "MINOR", "0", path);

		WritePrivateProfileString("IOLOC", "NAME", "data_out", path);
		WritePrivateProfileString("IOLOC", "RELATIVE_PATH", "data", path);
		GetFullPathName("data", 64, fpath, NULL);
		WritePrivateProfileString("IOLOC", "ABSOLUTE_PATH", fpath, path);
		WritePrivateProfileString("IOLOC", "EXTENSION", "CSV", path);

		WritePrivateProfileString("UPDATESRV", "PROTOCOL", "HTTP", path);
		WritePrivateProfileString("UPDATESRV", "URL", "http://example.com", path);

		WritePrivateProfileString("ACQSETTINGS", "DEFAULT_TIMER", "10000.0", path);

		WritePrivateProfileString("CTRLSETTINGS", "DEFAULT_DELTA", "0.001", path);
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
				DWORD error = GetPrivateProfileString(section, sub_sec, "N/A", buffer, 64, path);
				data[section][sub_sec] = buffer;
			}
		}

		//Version specification

		//IO Path location + name + extension

		//Update server protocol + url

		//Acquisition settings

		return data;
	}
}