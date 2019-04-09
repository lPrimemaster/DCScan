#include "CFlush.h"

const unsigned int CFlush::THREAD_CONCURRENCY = std::thread::hardware_concurrency();
std::mutex CFlush::gm;
HANDLE CFlush::hConsole_c;

int CFlush::rows = 0;
int CFlush::columns = 0;

CONSOLE_SCREEN_BUFFER_INFO CFlush::csbi;

std::vector<std::string> CFlush::str_split(const std::string &str, char ch)
{
	size_t pos = str.find(ch);
	size_t initialPos = 0;

	std::vector<std::string> rval;

	//Decompose
	while (pos != std::string::npos)
	{
		rval.push_back(str.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;
		pos = str.find(ch, initialPos);
	}

	///This is wrong for the case of '\n' char... unless ....
	//Add the last -- And prevent min from windef.h with parenthesis
	//rval.push_back(str.substr(initialPos, (std::min)(pos, str.size()) - initialPos + 1));

	return rval;
}

void CFlush::InitHandle(DWORD hnd)
{
	hConsole_c = GetStdHandle(hnd);

	//Assuming the console size can't be changed
	GetConsoleScreenBufferInfo(hConsole_c, &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void CFlush::InitHandle()
{
	hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);

	//Assuming the console size can't be changed
	GetConsoleScreenBufferInfo(hConsole_c, &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void CFlush::CloseHandle()
{
	::CloseHandle(hConsole_c);
}

void CFlush::FlushConsoleStream(std::stringstream * ss)
{
	std::string val = ss->str();
	std::vector<std::string> split = str_split(val, '\n');

	std::lock_guard<std::mutex> lock(gm);

#ifdef _WIN32
	SetConsoleActiveScreenBuffer(hConsole_c);

	for (int i = 0; i < split.size(); i++)
	{
		const char* buffer = split[i].c_str();
		COORD pos = { 0, i < rows - THREAD_CONCURRENCY - 1 ? i : rows - THREAD_CONCURRENCY };
		DWORD len = strlen(buffer);
		DWORD dwBytesWritten = 0;
		WriteConsoleOutputCharacter(hConsole_c, buffer, len, pos, &dwBytesWritten);
	}
#else
	//Linux code ???
#endif
	ss->clear();
}

void CFlush::FlushConsoleString(std::string & str)
{
	std::vector<std::string> split = str_split(str, '\n');

	std::lock_guard<std::mutex> lock(gm);

#ifdef _WIN32
	SetConsoleActiveScreenBuffer(hConsole_c);

	for (int i = 0; i < split.size(); i++)
	{
		const char* buffer = split[i].c_str();
		COORD pos = { 0, i < rows - THREAD_CONCURRENCY - 1 ? i : rows - THREAD_CONCURRENCY };
		DWORD len = strlen(buffer);
		DWORD dwBytesWritten = 0;
		WriteConsoleOutputCharacter(hConsole_c, buffer, len, pos, &dwBytesWritten);
	}
#else
	//Linux code ???
#endif
}

void CFlush::FlushConsoleCharLP(const char * buffer)
{
	std::string ex(buffer);
	std::vector<std::string> split = str_split(ex, '\n');

	std::lock_guard<std::mutex> lock(gm);

#ifdef _WIN32
	SetConsoleActiveScreenBuffer(hConsole_c);

	//Assuming the console size can be changed
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole_c, &csbi);
	int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	for (int i = 0; i < split.size(); i++)
	{
		const char* buffer = split[i].c_str();
		COORD pos = { 0, i };
		DWORD len = strlen(buffer);
		DWORD dwBytesWritten = 0;
		WriteConsoleOutputCharacter(hConsole_c, buffer, len, pos, &dwBytesWritten);
	}
#else
	//Linux code ???
#endif
}

void CFlush::ClearConsole(SHORT y, SHORT w)
{
	std::lock_guard<std::mutex> lock(gm);

#ifdef _WIN32
	//Assuming the console size can be changed
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole_c, &csbi);

	DWORD dwBytesWrittenSC = 0;
	FillConsoleOutputCharacter(hConsole_c, (TCHAR) ' ', csbi.dwSize.X * w,
		COORD{ 0, y }, &dwBytesWrittenSC);
#endif
}
