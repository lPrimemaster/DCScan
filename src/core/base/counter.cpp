#include "counter.h"
#include <iostream>
#include <functional>
#include <algorithm>

typedef bool (*CompareFunc)(float64, float64);

struct Cmpdata
{
	size_t index;
	enum Edge{ RISING, FALLING } edge;
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

//TODO: Fix equalities
std::tuple<uInt32, std::vector<size_t>> Counter::countPacket(float64 * data, size_t size, float64 barrier, float64 threshold)
{
	static float64 lastDataPoint = barrier + threshold;
	static Cmpdata::Edge lastEdge = Cmpdata::RISING;


	Cmpdata compare;
	compare.index = 0;
	uInt32 count = 0UL;
	std::vector<size_t> where;
	where.reserve(10); //When real tests are made change this for a likely value

	if (lastDataPoint < barrier - threshold && data[0] > barrier + threshold) [[unlikely]]
	{
		count++;
		where.push_back(0);
		compare.index = 1;
	}
	else if (lastDataPoint < barrier + threshold && lastDataPoint > barrier - threshold && lastEdge == Cmpdata::RISING) [[unlikely]]
	{
		compare.index = waitForLimit(data, 0, size, barrier + threshold, CMP_LARGER);
		if (compare.index > size)
		{
			lastDataPoint = data[size - 1];
			return std::make_pair(count, where);
		}
		else
		{
			where.push_back(compare.index);
			count++;
		}
	}

	while (true)
	{
		compare = waitForAny(data, compare.index, size, barrier, threshold);
		lastEdge = compare.edge;
		if (compare.index > size)
		{
			lastDataPoint = data[size - 1];
			return std::make_pair(count, where);
		}
		else if (compare.edge == Cmpdata::RISING)
		{
			where.push_back(compare.index);
			count++;
		}
	}
}
