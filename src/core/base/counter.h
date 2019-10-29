#pragma once
#include <NIDAQmx.h>
#include <iostream>
#include <tuple>
#include <vector>

namespace Counter
{
	//This function assures continuity of the data that is read
	std::tuple<uInt32, std::vector<size_t>> countPacket(float64* data, size_t size, float64 barrier, float64 threshold);
}