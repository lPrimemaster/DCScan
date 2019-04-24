#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <NIDAQmx.h>

//Handles an error from the NIDAQmx library
bool handleError(int32 error, const char* funcName);

// Returns in the closed interval [0, max]
long random_at_most(long max);

//Time based seed
void random_seed();

//Scroll console absolute
int ScrollByAbsoluteCoord(int iRows);

//Scroll console relative
int ScrollByRelativeCoord(int iRows);

//intpointer converter
template<typename T>
inline std::vector<intptr_t> convertToIntPointerH(T first)
{
	std::vector<intptr_t> ret;
	ret.push_back(reinterpret_cast<intptr_t>(first));
	return ret;
}
template<typename T, typename... Args>
inline std::vector<intptr_t> convertToIntPointerH(T first, Args... args)
{
	std::vector<intptr_t> ret = convertToIntPointerH(args...);
	ret.push_back(reinterpret_cast<intptr_t>(first));
	return ret;
}

//helper from algorithm -> std::reverse
template<typename T, typename... Args>
inline intptr_t* convertToIntPointer(T first, Args... args)
{
	std::vector<intptr_t> ret = convertToIntPointerH(first, args...);
	std::reverse(ret.begin(), ret.end());
	return ret.data();
}
