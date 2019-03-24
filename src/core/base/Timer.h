#pragma once
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

typedef std::chrono::high_resolution_clock HRClock;

struct Timestamp
{
	int year = 0;
	int month = 0;
	int day = 0;
	int hour = 0;
	int min = 0;
	int sec = 0;
	int millis = 0;
	int micros = 0;
	int nanos = 0;
};

class Timer
{
public:
	static inline std::string timeStampToString(Timestamp timestamp);

	static inline Timestamp apiTimeSystemHRC();
	static inline long long apiTimeSystemHRC_Nano();

	static inline Timestamp apiTimeSystemHRC_NanoToTimestamp(long long ns);
	static inline Timestamp apiTimeSystemHRC_DeltaToTimestamp(long long delta);

	static inline std::string apiUptimeString();
	static inline std::string apiTimeSystemClockString();

private:
	Timer() = default;
	~Timer() = default;

private:
	static const std::chrono::system_clock::time_point start;
};

inline std::string Timer::timeStampToString(Timestamp timestamp)
{
	const Timestamp ts = timestamp;

	std::stringstream ss;
	ss << ts.year << "/" << ts.month << "/" << ts.day << " " << ts.hour << ":" << ts.min << ":" << ts.sec << ":" << ts.millis << ":" << ts.micros << ":" << ts.nanos;
	std::string result = ss.str();

	return result;
}

inline std::string Timer::apiTimeSystemClockString()
{
	time_t tn = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	tm utc;
	gmtime_s(&utc, &tn);

	std::stringstream ss;

	ss << utc.tm_year + 1900 << "-" << utc.tm_mon + 1 << "-" << utc.tm_mday << " " << utc.tm_hour << ":" << utc.tm_min << ":" << (utc.tm_sec < 10 ? "0" : "") << utc.tm_sec;

	std::string result = ss.str();

	return result;
}

inline Timestamp Timer::apiTimeSystemHRC()
{
	std::chrono::system_clock::time_point sctp = std::chrono::system_clock::now();
	auto tp = sctp.time_since_epoch();
	typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>::type> days;

	days d = std::chrono::duration_cast<days>(tp);
	tp -= d;
	std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(tp);
	tp -= h;
	std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(tp);
	tp -= m;
	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(tp);
	tp -= s;
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp);
	tp -= ms;
	std::chrono::microseconds us = std::chrono::duration_cast<std::chrono::microseconds>(tp);
	tp -= us;
	std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(tp);

	time_t time_n = time(NULL);
	struct tm tn;
	localtime_s(&tn, &time_n);

	Timestamp stamp;
	stamp.year = tn.tm_year + 1900;
	stamp.month = tn.tm_mon + 1;
	stamp.day = tn.tm_mday;
	stamp.hour = tn.tm_hour;
	stamp.min = m.count();
	stamp.sec = s.count();
	stamp.millis = ms.count();
	stamp.micros = us.count();
	stamp.nanos = ns.count();

	return stamp;
}

inline long long Timer::apiTimeSystemHRC_Nano()
{
	std::chrono::system_clock::time_point sctp = std::chrono::system_clock::now();
	auto tp = sctp.time_since_epoch();

	std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(tp);

	return ns.count();
}

inline Timestamp Timer::apiTimeSystemHRC_NanoToTimestamp(long long ns)
{
	std::chrono::nanoseconds c_ns = std::chrono::nanoseconds(ns);
	typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>::type> days;

	days d = std::chrono::duration_cast<days>(c_ns);
	c_ns -= d;
	std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(c_ns);
	c_ns -= h;
	std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(c_ns);
	c_ns -= m;
	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(c_ns);
	c_ns -= s;
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(c_ns);
	c_ns -= ms;
	std::chrono::microseconds us = std::chrono::duration_cast<std::chrono::microseconds>(c_ns);
	c_ns -= us;
	std::chrono::nanoseconds l_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(c_ns);

	time_t time_n = time(NULL);
	struct tm tn;
	localtime_s(&tn, &time_n);

	Timestamp stamp;
	stamp.year = tn.tm_year + 1900;
	stamp.month = tn.tm_mon + 1;
	stamp.day = tn.tm_mday;
	stamp.hour = tn.tm_hour;
	stamp.min = m.count();
	stamp.sec = s.count();
	stamp.millis = ms.count();
	stamp.micros = us.count();
	stamp.nanos = l_ns.count();

	return stamp;
}

//This delta must be smaller then 60 mins in nanoseconds
inline Timestamp Timer::apiTimeSystemHRC_DeltaToTimestamp(long long delta)
{
	std::chrono::nanoseconds c_ns = std::chrono::nanoseconds(delta);
	typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>::type> days;

	std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(c_ns);
	c_ns -= m;
	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(c_ns);
	c_ns -= s;
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(c_ns);
	c_ns -= ms;
	std::chrono::microseconds us = std::chrono::duration_cast<std::chrono::microseconds>(c_ns);
	c_ns -= us;
	std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(c_ns);

	Timestamp stamp;
	stamp.min = m.count();
	stamp.sec = s.count();
	stamp.millis = ms.count();
	stamp.micros = us.count();
	stamp.nanos = ns.count();

	return stamp;
}

inline std::string Timer::apiUptimeString()
{
	std::chrono::system_clock::time_point sctp = std::chrono::system_clock::now();
	std::chrono::system_clock::duration tp = sctp - start;

	typedef std::chrono::duration<int, std::ratio_multiply<std::chrono::hours::period, std::ratio<24>>::type> days;
	
	days d = std::chrono::duration_cast<days>(tp);
	tp -= d;
	std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(tp);
	tp -= h;
	std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(tp);
	tp -= m;
	std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(tp);
	tp -= s;

	std::stringstream ss;

	ss << d.count() << " days " << h.count() << " hours " << m.count() << " minutes " << s.count() << " seconds";

	std::string result = ss.str();

	return result;
}
