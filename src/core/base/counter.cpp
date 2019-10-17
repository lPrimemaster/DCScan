#include "counter.h"
#include <iostream>
#include <functional>
#include <algorithm>

typedef bool (*CompareFunc)(float64, float64);

struct Cmpdata
{
	size_t index;
	enum { RISING, FALLING }edge;
};

size_t waitForLimit(float64* data, size_t start, size_t size, float64 barrier, CompareFunc cmp)
{
	size_t idx = start;
	while (true)
	{
		if (cmp(data[idx], barrier)) 
			return idx;
		else if (idx < size)
		{
			idx++;
			continue;
		}
		return size + 1;
	}
}

#define CMP_LARGER  [](float64 a, float64 b) -> bool { return a > b; }
#define CMP_SMALLER [](float64 a, float64 b) -> bool { return a < b; }

Cmpdata waitForAny(float64* data, size_t start, size_t size, float64 barrier, float64 noise_threshold)
{
	Cmpdata cd = Cmpdata();

	if (data[start] > barrier - noise_threshold)
	{
		cd.index = waitForLimit(data, start, size, barrier - noise_threshold, CMP_SMALLER);
		cd.edge = Cmpdata::FALLING;
	}
	else if (data[start] < barrier + noise_threshold)
	{
		cd.index = waitForLimit(data, start, size, barrier + noise_threshold, CMP_LARGER);
		cd.edge = Cmpdata::RISING;
	}

	return cd;
}

uInt32 Counter::countPacket(float64 * data, size_t size, float64 barrier, float64 threshold)
{
	static float64 lastDataPoint = barrier + threshold;
	Cmpdata compare;

	compare.index = 0;
	uInt32 count = 0UL;

	if (lastDataPoint < barrier + threshold && data[0] > barrier + threshold)
	{
		count++;
		compare.index = 1;
	}

	while (true)
	{
		compare = waitForAny(data, compare.index, size, barrier, threshold);
		if (compare.index > size)
		{
			lastDataPoint = data[size - 1];
			return count;
		}
		else if (compare.edge == Cmpdata::RISING)
		{
			count++;
		}
	}
}
