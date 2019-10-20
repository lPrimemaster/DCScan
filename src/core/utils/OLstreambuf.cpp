#include "OLstreambuf.h"
#include "../../ctrl/CFlush.h"

OLstreambuf::OLstreambuf()
{
	h = CFlush::getDefaultHandle();
}

std::streambuf::int_type OLstreambuf::overflow(int_type c)
{
	if (c != EOF) 
	{
		/*DWORD written;
		SetConsoleCursorPosition(h, { 0, 0 });
		WriteConsole(h, &c, 1, &written, nullptr);*/
		std::string out(1, c);
		CFlush::println(out.c_str());
	}
	return c;
}

std::streamsize OLstreambuf::xsputn(char_type const* s, std::streamsize count)
{
	DWORD written = 0;
	/*SetConsoleCursorPosition(h, { 0, 0 });
	WriteConsole(h, s, count, &written, nullptr);*/
	CFlush::println(s);
	return written;
}