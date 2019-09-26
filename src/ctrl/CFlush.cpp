#include "CFlush.h"
#include <ios>
#include <io.h>
#include <fcntl.h>
#include <fstream>

HANDLE CFlush::handle[2] = { nullptr };
SHORT CFlush::columns = 0;
SHORT CFlush::rows = 0;

static char* convert(unsigned int num, int base)
{
	static char Representation[] = "0123456789ABCDEF";
	static char buffer[50];
	char* ptr;

	ptr = &buffer[49];
	*ptr = '\0';

	do
	{
		*--ptr = Representation[num % base];
		num /= base;
	} while (num != 0);

	return(ptr);
}

//Sampled from : http://www.firmcodes.com/write-printf-function-c/
static void custom_printf(WORD att, COORD coord, const char* fmt, va_list arg)
{
	char* traverse;
	unsigned int i;
	int a;
	char* s;
	COORD where = coord;

	auto fastwc = [&](const char* c, DWORD size, WORD color) -> void
	{
		for (int i = 0; i < size; i++)
		{
			CFlush::writeCharAttrib(CFlush::handle[0], c++, 1, where, color);
			if (where.X++ > CFlush::columns)
			{
				where.X = 0;
				where.Y++;
			}
		}
	};

	//1 - Init arguments
	for (traverse = (char*)fmt; *traverse != '\0'; traverse++)
	{
		while (*traverse != '%')
		{
			if (*traverse == '\0')
			{
				return;
			}
			else if (*traverse == '\n')
			{
				where.X = 0;
				where.Y++;
				traverse++;
			}

			fastwc(traverse, 1, att);
			traverse++;
		}

		traverse++;

		//2 - fetch and exec args
		switch (*traverse)
		{
		case 'c': i = va_arg(arg, int); //Fetch char argument
			fastwc(std::to_string(i).c_str(), 1, att);
			break;

		case 'd': a = va_arg(arg, int); //Fetch Decimal/Integer argument
			if (a < 0)
			{
				i = -a;
				fastwc("-", 1, att);
			}
			else
			{
				i = a;
			}
			s = convert(i, 10);
			fastwc(s, strlen(s), att);
			break;

		case 'o': i = va_arg(arg, unsigned int); //Fetch Octal representation
			s = convert(i, 8);
			fastwc(s, strlen(s), att);
			break;

		case 's': s = va_arg(arg, char*); 		 //Fetch string
			fastwc(s, strlen(s), att);
			break;

		case 'x': i = va_arg(arg, unsigned int); //Fetch Hexadecimal representation
			s = convert(i, 16);
			fastwc(s, strlen(s), att);
			break;
		}

	}
}

bool CFlush::Init()
{
	//One serves as a backbuffer as the other serves as the front one

	handle[0] = CreateConsoleScreenBuffer(GENERIC_WRITE, NULL, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	handle[1] = CreateConsoleScreenBuffer(GENERIC_WRITE, NULL, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	//Assuming the console size won't be changed
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	////Redirect std::ios and stdio to this handle[0]
	//int hConHandle = _open_osfhandle((long)handle[0], _O_TEXT);
	//FILE* fp = _fdopen(hConHandle, "t");
	//*stdout = *fp;
	//setvbuf(stdout, NULL, _IONBF, 0);

	//std::ifstream stream(fp);
	//std::cout.rdbuf(stream.rdbuf());

	return handle[0] != nullptr;
}

bool CFlush::writeCharAttrib(HANDLE hnd, LPCSTR str, DWORD size, COORD location, WORD color)
{
	WORD att = color;
	DWORD numberAttrbWritten = 0;
	DWORD numberCharsWritten = 0;

	WriteConsoleOutputAttribute(hnd, &att, size, location, &numberAttrbWritten);
	WriteConsoleOutputCharacter(hnd, str, size, location, &numberCharsWritten);

	return numberAttrbWritten == numberCharsWritten;
}

bool CFlush::println(int line, const char* fmt, ...)
{
	COORD pos = {0, line};
	va_list arg;
	va_start(arg, fmt);

	printPos(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, pos, fmt, arg);

	va_end(arg);

	return true;
}

bool CFlush::printlnColor(Color c, int line, const char* fmt, ...)
{
	COORD pos = { 0, line };
	va_list arg;
	va_start(arg, fmt);

	printPos((WORD)c, pos, fmt, arg);

	va_end(arg);

	return true;
}

bool CFlush::printXY(COORD xy, const char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	printPos(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY, xy, fmt, arg);

	va_end(arg);

	return true;
}

bool CFlush::printXYColor(Color c, COORD xy, const char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);

	printPos((WORD)c, xy, fmt, arg);

	va_end(arg);

	return true;
}

bool CFlush::printPos(WORD att, COORD pos, const char* fmt, va_list arg)
{

	SetConsoleActiveScreenBuffer(handle[0]);
	custom_printf(att, pos, fmt, arg);

	fflush(stdout);

	return true;
}

std::string CFlush::formatString(const char* fmt, ...)
{
	char local_buff[4096];

	va_list args;

	va_start(args, fmt);

	vsprintf_s(local_buff, fmt, args);

	va_end(args);

	return std::string(local_buff);
}

HANDLE CFlush::getDefaultHandle()
{
	return handle[0];
}
