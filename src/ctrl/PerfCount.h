#pragma once

#include <iostream>
#include <atomic>
#include <vector>
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
	double value = 0.0;
	WCHAR cname[64];
};

struct CounterInfo
{
	PCWSTR counterName = NULL;
	PDH_HCOUNTER counter = NULL;
	std::vector<Log> logs;
	double latest = 0.0;
};

class PerfCount
{
public:
	static void Init();
	static void PrintValidProcTimes();
	static void AddCounter(PCWSTR name);
	static void Record(std::atomic<int>* flags, void* data);

	static double GetLatestCpuUsage();

private:
	static PDH_HQUERY query;
	static HANDLE Event;

	static int time;

	static bool fIsWorking;

	static std::vector<CounterInfo> vciSelectedCounters;
};

