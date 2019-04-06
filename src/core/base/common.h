#pragma once
#include <iostream>
#include <NIDAQmx.h>

//Handles a error from the NIDAQmx library
bool handleError(int32 error, const char* funcName);

// Returns in the closed interval [0, max]
long random_at_most(long max);

//Time based seed
void random_seed();

//Scroll console absolute
int ScrollByAbsoluteCoord(int iRows);

//Scroll console relative
int ScrollByRelativeCoord(int iRows);

//Custom printf to work with WinApi
void wc_flush(std::stringstream * ss);