#pragma once
#include <vector>
#include <mutex>
#include <iostream>
#include <sstream>
#include <algorithm>
#ifdef _WIN32
#include <Windows.h>
#endif

//This class supports async console flush (even for multiple instances of class) -- default handle = STD_OUTPUT_HANDLE
class CFlush
{
public:
	static void InitHandle(DWORD hnd);
	static void InitHandle();
	static void CloseHandle();

	static void FlushConsoleStream(std::stringstream * ss);
	static void FlushConsoleString(std::string& str);
	static void FlushConsoleCharLP(const char* buffer);
	static void ClearConsole(SHORT y, SHORT w);

public:
	static int rows;
	static int columns;

private:
	CFlush() = default;
	~CFlush() = default;

	static std::vector<std::string> str_split(const std::string &str, char ch);

private:
	static std::mutex gm;
	static HANDLE hConsole_c;

	static CONSOLE_SCREEN_BUFFER_INFO csbi;

	static const unsigned int THREAD_CONCURRENCY;
};

