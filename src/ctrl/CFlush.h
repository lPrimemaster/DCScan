#pragma once
#include <vector>
#include <mutex>
#include <iostream>
#include <sstream>
#include <algorithm>
#ifdef _WIN32
#include <Windows.h>
#endif

//TODO: Make ypos flexible when console is full - restart from top
//This class supports async console flush (even for multiple instances of class) -- default handle = STD_OUTPUT_HANDLE
class CFlush
{
public:
	enum class Color
	{
		TEXT_WHITE = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		TEXT_RED   = FOREGROUND_RED   | FOREGROUND_INTENSITY,
		TEXT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		TEXT_BLUE  = FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
		TEXT_GRAY  = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};

	friend static void custom_printf(WORD att, COORD coord, const char* fmt, va_list arg);
	static bool Init();
	static bool println(const char* fmt, ...);
	static bool println(int line, const char* fmt, ...);
	static bool printlnColor(Color c, const char* fmt, ...);
	static bool printlnColor(Color c, int line, const char* fmt, ...);
	static bool printXY(COORD xy, const char* fmt, ...);
	static bool printXYColor(Color c, COORD xy, const char* fmt, ...);



	static std::string formatString(const char* fmt, ...);

	static HANDLE getDefaultHandle();

public:
	static SHORT columns;
	static SHORT rows;
	static SHORT current_ypos;

private:
	CFlush() = default;
	~CFlush() = default;

	static bool writeCharAttrib(HANDLE hnd, LPCSTR str, DWORD size, COORD location, WORD color);
	static bool printPos(WORD att, COORD pos, const char* fmt, va_list arg);

	//Prints everything in a specific line - no color

	static HANDLE handle[2];

};

