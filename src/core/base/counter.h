#pragma once
#include <NIDAQmx.h>

namespace Counter
{
	//This function assures continuity of the data that is read
	uInt32 countPacket(float64* data, size_t size, float64 barrier, float64 threshold);
}