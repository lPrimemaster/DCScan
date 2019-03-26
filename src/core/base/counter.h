#pragma once
#include <NIDAQmx.h>

namespace Counter
{
	uInt32 schmittTriggerData(float64 * data, size_t size, float64 lowerBound, float64 upperBound);
}