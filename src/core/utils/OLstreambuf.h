#pragma once
#include <iostream>
#include <Windows.h>

//Original code picked from https://stackoverflow.com/questions/39110882/redirecting-cout-to-the-new-buffer-that-created-with-winapi
//thanks to Jerry Coffin
//Further modified
class OLstreambuf : public std::streambuf
{
public:
	OLstreambuf();
protected:
	virtual int_type overflow(int_type c) override;

	virtual std::streamsize xsputn(char_type const* s, std::streamsize count) override;

private:
	HANDLE h;
};

