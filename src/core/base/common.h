#pragma once
#include <iostream>
#include <algorithm>
#include <sstream>
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

//Get NI devices in the system
std::vector<std::string> getNIDevices();

//int pointer converter
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

	intptr_t* out = new intptr_t[ret.size()];

	for (int i = 0; i < ret.size(); i++)
	{
		out[i] = ret[i];
	}

	return out;
}

inline std::string tostring_prec(const float value, const int precision = 4)
{
	std::ostringstream out;
	out.precision(precision);
	out << std::fixed << value;
	return out.str();
}

//Returns orderly arguments from the pointer - not overflow protected!
template<typename First>
inline std::tuple<First> from_intPointerH(intptr_t* ptr, First f)
{
	return std::make_tuple<First>(reinterpret_cast<First>(*ptr));
}

template<typename First, typename... Args>
inline std::tuple<First, Args...> from_intPointerH(intptr_t* ptr, First f, Args... args)
{
	auto tuple = std::make_tuple<First>(reinterpret_cast<First>(*ptr));
	return std::tuple_cat(std::move(tuple), from_intPointerH<Args...>(++ptr, args...));
}

#define REP0(X)
#define REP1(X) , X
#define REP2(X) REP1(X) , X
#define REP3(X) REP2(X) , X
#define REP4(X) REP3(X) , X
#define REP5(X) REP4(X) , X
#define REP6(X) REP5(X) , X
#define REP7(X) REP6(X) , X
#define REP8(X) REP7(X) , X
#define REP9(X) REP8(X) , X
#define REP10(X) REP9(X) , X

#define REPM(X) \
	REP##X(nullptr)
#define SUFIX_CAT(X, Y) X ## Y

#define VA_NARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define COUNT_ARGS(...)   VA_NARGS(__VA_ARGS__)

#define declare_fip(X) \
template<typename First, typename... Args> \
inline std::tuple<First, Args...> from_intPointer##X(intptr_t* ptr) \
{ \
	return from_intPointerH<First, Args...>(ptr REPM(X)); \
} \

declare_fip(0)
declare_fip(1)
declare_fip(2)
declare_fip(3)
declare_fip(4)
declare_fip(5)
declare_fip(6)
declare_fip(7)
declare_fip(8)
declare_fip(9)
declare_fip(10)

//TODO: Define the size accounting the __VA_ARGS__ size
#define from_intPointer(size, ...) SUFIX_CAT(from_intPointer, size)<__VA_ARGS__>