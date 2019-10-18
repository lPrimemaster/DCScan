#pragma once
#include "Task.h"
#include "base/Timer.h"
#include <mutex>
#include <deque>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

//Align to closest power of 2 of 20 (if working with raw memory) [unknown alignment]
struct /*alignas(32)*/ DataPacket
{
	float64* data;
	size_t data_size;
	int32 data_read;
	long long software_tor_ns;
};

struct CallbackPacket
{
	static std::mutex* getGlobalCBPMutex()
	{
		static std::mutex intance;
		return &intance;
	}

	static std::deque<DataPacket>* getGlobalCBPStack()
	{
		static std::deque<DataPacket> instance;
		return &instance;
	}
};

struct CallBackRegistries
{
	static inline py::function data_count_callback;
};

int32 __cdecl EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);

void registerDataCounter(std::string function, std::string module);