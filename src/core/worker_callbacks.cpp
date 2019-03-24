#include "worker_callbacks.h"

#include "base/Timer.h"

int32 __cdecl EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
	static int   totalRead = 0;
	static int   totalPacket = 0;
	int32 read = 0;

	//Make thread safe operations post this line
	std::lock_guard<std::mutex> lock(*CallbackPacket::getGlobalCBPMutex());

	DataPacket pct;
	pct.data = (float64*)malloc(sizeof(float64) * nSamples);
	pct.data_size = nSamples;

	//Supressing error handling for speed
	DAQmxReadAnalogF64(taskHandle, nSamples, 10.0, DAQmx_Val_GroupByScanNumber, pct.data, nSamples, &pct.data_read, NULL);
	//DAQmx Reference Manual suggests to read Tos_ret (software tor) here
	pct.software_tor_ns = Timer::apiTimeSystemHRC_Nano();

	//Produce value into the deque
	CallbackPacket::getGlobalCBPStack()->push_back(pct);

	return 0;
}