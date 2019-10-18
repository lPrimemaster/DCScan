#pragma once

#include <iostream>
#include <atomic>
#include <unordered_map>
#define _UNICODE
#define UNICODE
#include <Windows.h>
#include <Psapi.h>
#include <string.h>
#include <tchar.h>
#include <Pdh.h>

#define SIZE 1024

#define SAMPLE_INTERVAL 1

//Partially sampled from wincode website (refactored - César Godinho)

struct Log
{
	int time = 0;
	std::atomic<double> value = 0.0;
	std::atomic<double> mean = 0.0;
	WCHAR cname[64];
};

struct CounterInfo
{
	PCWSTR counterName = NULL;
	PDH_HCOUNTER counter = NULL;
	Log latest;
};

class PerfCount
{
public:
	enum class CounterID : int;
	static void Init();
	static void Deinit();
	static void PrintValidProcTimes();
	static void AddCounter(PCWSTR name, CounterID identifier);
	static void Record(std::atomic<int>* flags, void* data);

	static double getCounterValue(CounterID identifier);

	enum class CounterID
	{
		PROCESSOR_TIME, MEMCOPIES_SEC
	};

private:
	static PDH_HQUERY query;
	static HANDLE Event;

	static int time;

	static bool fIsWorking;

	static std::unordered_map<CounterID, CounterInfo*> vciSelectedCounters;
};

