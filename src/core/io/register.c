#include "register.h"

void createIniFile(LPCTSTR name)
{
	IniFileProperties ini_props =
	{
		.section = { "VERSION", "IOLOC", "UPDATESRV", "ACQSETTINGS", "CTRLSETTINGS" },
		.sub_sec =
		{
			{ "MAJOR", "MINOR" },				//VERSION
			{ "NAME", "PATH", "EXTENSION" },	//IOLOC
			{ "PROTOCOL", "URL" },				//UPDATESERV
			{ "" },								//ACQSETTINGSS
			{ "" }								//CTRLSETTINGS
		}
	};
}

void readIniFile(LPCTSTR name)
{
	IniFileProperties ini_props =
	{
		.section = { "VERSION", "IOLOC", "UPDATESRV", "ACQSETTINGS", "CTRLSETTINGS" },
		.sub_sec = 
		{
			{ "MAJOR", "MINOR" },				//VERSION
			{ "NAME", "PATH", "EXTENSION" },	//IOLOC
			{ "PROTOCOL", "URL" },				//UPDATESERV
			{ "" },								//ACQSETTINGSS
			{ "" }								//CTRLSETTINGS
		}
	};

	//64 bytes max string on ini file
	char fullData[5][3][64];

	for (int i = 0; i < sizeof(ini_props.section) / sizeof(ini_props.section[0]); i++)
	{
		for (int k = 0; k < sizeof(ini_props.sub_sec[i]) / sizeof(ini_props.sub_sec[i][k]); k++)
		{
			DWORD error = GetPrivateProfileString(ini_props.section[i], ini_props.sub_sec[i][k], "N/A", fullData[i][k], 64, name);
		}
	}

	//Parse the read data - ordered - FIXME
	IniFileData ini_out =
	{
		.version_major = atoi(fullData[0][0]),
		.version_minor = atoi(fullData[0][1]),

		.ioloc_name = fullData[1][0],
		.ioloc_path = fullData[1][1],
		.ioloc_exts = fullData[1][2],

		.updt_prt = fullData[2][0],
		.updt_url = fullData[2][1]
	};
}