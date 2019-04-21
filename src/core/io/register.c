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
	IniFileData ini_out;

	strcpy(ini_out.);
}