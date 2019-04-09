#include "common.h"
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <sstream>
#include <thread>
#include <vector>
#include <algorithm>
#include <mutex>
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32


bool handleError(int32 error, const char * funcName)
{
	if (error < 0)
	{
		char buffer[2048];
		DAQmxGetExtendedErrorInfo(buffer, 2048);
		std::cerr << funcName << " returned error: " << buffer << std::endl;
		return true;
	}
	else if (error > 0)
	{
		char buffer[2048];
		DAQmxGetExtendedErrorInfo(buffer, 2048);
		std::cerr << funcName << " returned warning: " << buffer << std::endl;
		return true;
	}
	return false;
}


long random_at_most(long max) 
{
	unsigned long num_bins = (unsigned long)max + 1;
	unsigned long num_rand = (unsigned long)RAND_MAX + 1;
	unsigned long bin_size = num_rand / num_bins;
	unsigned long	defect = num_rand % num_bins;

	long x;
	do {
		x = rand();
	}
	// This is carefully written not to overflow
	while (num_rand - defect <= (unsigned long)x);

	// Truncated division is intentional
	return x / bin_size;
}

void random_seed()
{
	srand(time(NULL));
}

int ScrollByAbsoluteCoord(int iRows)
{
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	SMALL_RECT srctWindow;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	// Get the current screen buffer size and window position. 

	if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
	{
		printf("GetConsoleScreenBufferInfo (%d)\n", GetLastError());
		return 0;
	}

	// Set srctWindow to the current window size and location. 

	srctWindow = csbiInfo.srWindow;

	// Check whether the window is too close to the screen buffer top

	if (srctWindow.Top >= iRows)
	{
		srctWindow.Top -= (SHORT)iRows;     // move top up
		srctWindow.Bottom -= (SHORT)iRows;  // move bottom up

		if (!SetConsoleWindowInfo(
			hStdout,          // screen buffer handle 
			TRUE,             // absolute coordinates 
			&srctWindow))     // specifies new location 
		{
			printf("SetConsoleWindowInfo (%d)\n", GetLastError());
			return 0;
		}
		return iRows;
	}
	else
	{
		printf("\nCannot scroll; the window is too close to the top.\n");
		return 0;
	}
}

int ScrollByRelativeCoord(int iRows)
{
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	SMALL_RECT srctWindow;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	// Get the current screen buffer window position. 

	if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
	{
		printf("GetConsoleScreenBufferInfo (%d)\n", GetLastError());
		return 0;
	}

	// Check whether the window is too close to the screen buffer top

	if (csbiInfo.srWindow.Top >= iRows)
	{
		srctWindow.Top = -(SHORT)iRows;     // move top up
		srctWindow.Bottom = -(SHORT)iRows;  // move bottom up 
		srctWindow.Left = 0;         // no change 
		srctWindow.Right = 0;        // no change 

		if (!SetConsoleWindowInfo(
			hStdout,          // screen buffer handle 
			FALSE,            // relative coordinates
			&srctWindow))     // specifies new location 
		{
			printf("SetConsoleWindowInfo (%d)\n", GetLastError());
			return 0;
		}
		return iRows;
	}
	else
	{
		printf("\nCannot scroll; the window is too close to the top.\n");
		return 0;
	}
}
