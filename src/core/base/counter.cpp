#include "counter.h"
#include <iostream>

int waitForLowerBound(float64 * data, size_t startingIndex, size_t size, float64 lowerBound)
{
	size_t idx = startingIndex;
	while (true)
	{
		if (data[idx] < lowerBound)
			break;
		else if (idx < size)
		{
			idx++;
			continue;
		}
		return -1;
	}
	return idx;
}

int waitForLowerUpperBound(float64* data, size_t startingIndex, size_t size, float64 lowerBound, float64 upperBound, bool* lower)
{
	size_t idx = startingIndex;
	while (true)
	{
		if (data[idx] < lowerBound)
		{
			*lower = true;
			break;
		}
		else if (data[idx] > upperBound)
		{
			*lower = false;
			break;
		}
		else if (idx < size)
		{
			idx++;
			continue;
		}
		*lower = false;
		return -1;
	}
	return idx;
}

uInt32 Counter::schmittTriggerData(float64 * data, size_t size, float64 lowerBound, float64 upperBound)
{
	static float64 lastDataPoint = lowerBound;
	uInt32 count = 0;
	size_t index = 0;

	static bool overshoot = false;

	//define first entry point
	if (data[0] < lowerBound && lastDataPoint > lowerBound && !overshoot)
	{
		count++;
		//std::cout << "[Default-last] Count " << count << " at point (" << index / 100.0 << "," << data[index] << ")." << std::endl;
	}
	else if (data[0] > upperBound)
	{
		index = waitForLowerBound(data, 0, size, lowerBound);
		overshoot = false;
	}
	else if (data[0] < upperBound)
	{
		if (!overshoot)
		{
			bool lower;
			index = waitForLowerUpperBound(data, 0, size, lowerBound, upperBound, &lower);

			if (!lower)
			{
				index = waitForLowerBound(data, index, size, lowerBound);
			}
			else if (lastDataPoint > lowerBound)
			{
				count++;
				//std::cout << "[Overshoot-last] Count " << count << " at point (" << index / 100.0 << "," << data[index] << ")." << std::endl;
			}
		}
		else
		{
			index = waitForLowerBound(data, 0, size, lowerBound);
			overshoot = false;
		}
	}

	lastDataPoint = data[size - 1];

	//define main loop
	while (true)
	{
		if (index >= size)
			break;
		//Data is always lower bound at this point
		if (data[index] > lowerBound && data[index] < upperBound)
		{
			bool lower;
			index = waitForLowerUpperBound(data, index, size, lowerBound, upperBound, &lower);

			if (index == -1)
			{
				overshoot = false;
				break;
			}

			if (!lower)
			{
				overshoot = true;
				index = waitForLowerBound(data, index, size, lowerBound);

				if (index == -1)
					break;

				index++;
				continue;
			}
			else
			{
				count++;
				//std::cout << "[Default] Count " << count << " at point (" << index / 100.0 << "," << data[index] << ")." << std::endl;
				index++;
				continue;
			}
		}

		index++;
	}

	return count;
}