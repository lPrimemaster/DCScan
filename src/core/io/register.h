#pragma once
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif
	typedef struct _IniFileData
	{
		const unsigned version_major;
		const unsigned version_minor;

		const char* ioloc_name;
		const char* ioloc_path;
		const char* ioloc_exts = fullData[1][2];

		const char* updt_prt;
		const char* updt_url;
	}IniFileData;

	typedef struct _IniFileProperties 
	{

		const char* section[5];

		const char* sub_sec[5][3];

		//ordered in sub_sec
		//const char* version[2];
		//const char* iolocat[3];
		//const char* updtsrv[2];
		//const char* acqsett[2];
		//const char* ctrsett[2];

	}IniFileProperties;

	void createIniFile(LPCTSTR name);
	void readIniFile(LPCTSTR name);

#ifdef __cplusplus
}
#endif
