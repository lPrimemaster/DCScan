#include "worker_threads.h"
#include "worker_callbacks.h"
#include "base/counter.h"
#include "base/Timer.h"
#include "memory/DataChunk.h"
#include "../ctrl/CFlush.h"
#include "../ctrl/PyScript.h"
#include <chrono>

void acquireThread(std::atomic<int>* flags, void * data)
{
	AcquireDataOptions * ado = reinterpret_cast<AcquireDataOptions*>(data);

	Task task(ado->tproperties.name);

	task.addChannel(Task::VoltageChannel, ado->tproperties.channel);
	task.addTimer(ado->tproperties.timer);
	task.addEventCallback(EveryNCallback, nullptr, ado->tproperties.timer.samplesPerChannel, 0);

	task.start(); //This marks the starting time of acquisition (NI-DAQmx reference manual)

	while (flags->load() == THREAD_RUN)
	{
		std::this_thread::yield();
	}

	task.stop();

	flags->store(THREAD_ENDED);
}

void processThread(std::atomic<int>* flags, void * data)
{
	intptr_t* int_ptr = reinterpret_cast<intptr_t*>(data);

	FILE* f = reinterpret_cast<FILE*>(int_ptr[0]);
	AcquireDataOptions* opt = reinterpret_cast<AcquireDataOptions*>(int_ptr[1]);

	while (true)
	{
		while (CallbackPacket::getGlobalCBPStack()->empty() && flags->load() == THREAD_RUN)
		{
			//Use sleep_for() or yield() ? 
			//TODO: Make this wait value based on the frequency of acquisition
			std::this_thread::sleep_for(std::chrono::microseconds(10));
		}

		if (flags->load() == THREAD_HALT && CallbackPacket::getGlobalCBPStack()->empty())
			break;


		//Get ready to process data
		CallbackPacket::getGlobalCBPMutex()->lock();
		auto cbp_stack = CallbackPacket::getGlobalCBPStack();
		auto dpacket = cbp_stack->front();
		cbp_stack->pop_front();
		CallbackPacket::getGlobalCBPMutex()->unlock();

		static DataChunk<uInt32,    1> dd_mem_chunk(dpacket.data_size, DEFAULT_DATA);
		static DataChunk<long long, 1> td_mem_chunk(dpacket.data_size,    TIME_DATA);

		//Process the data
		static const long long dt = 1000000000 / opt->tproperties.timer.sampleRate; //In nanoseconds
		static int dpser = 0;

		long long ns = dpacket.software_tor_ns;
		std::vector<long long> local_ns_table;
		local_ns_table.reserve(dpacket.data_size);

		auto getLocal_ns = [ns, dpacket](size_t i) -> long long { return ns - dt * (dpacket.data_size - i); };

		if (dpacket.data != nullptr)
		{
			for (int i = 0; i < dpacket.data_size; i++)
			{
				long long local_ns = getLocal_ns(i);
				Timestamp ts = Timer::apiTimeSystemHRC_NanoToTimestamp(local_ns);
				local_ns_table.push_back(local_ns);

				fprintf(f, "%d,%d,%d,%lf,%s\n", dpser, i, 1000 * dpser + i, dpacket.data[i], Timer::timeStampToString(ts).c_str());
			}
			dpser++;

			//Noise threshold is equal bilateral - 1.0 => TODO: Make this unilateral specific
			//TODO: Normalize counts per unit time
			uInt32 count = Counter::countPacket(dpacket.data, dpacket.data_size, 5.0, 1.0);

			dd_mem_chunk.add(count); //This overflows like this
			td_mem_chunk.set(local_ns_table.data());

			//TODO: Count data and send it to front end as a callback
			if (CallBackRegistries::data_count_callback.ptr() && PyScript::getAtomicState())
			{
				py::gil_scoped_acquire acquire;
				CallBackRegistries::data_count_callback(count, 1, ns);
				py::gil_scoped_release release;
			}

			//Free data copy
			free(dpacket.data);

		}
	}

	flags->store(THREAD_ENDED);
}

void controlThread(std::atomic<int>* flags, void* data)
{
	int x = 0;

	while (true)
	{
		if (flags->load() == THREAD_HALT)
			break;

		Timestamp ts = Timer::apiTimeSystemHRC();
		CFlush::println(1, "Current Time: %s", CFlush::formatString("%02d:%02lld:%02lld", ts.hour, ts.min, ts.sec).c_str());
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		if (CallBackRegistries::data_count_callback.ptr() && PyScript::getAtomicState())
		{
			try
			{
				py::gil_scoped_acquire acquire;
				CallBackRegistries::data_count_callback(std::sin(x / 100.0f), int(x / 10.0), 0.1);
				py::gil_scoped_release release;
				x++;
			}
			catch (py::error_already_set & eas)
			{
				std::cerr << eas.what() << std::endl;
			}
		}
	}
	flags->store(THREAD_ENDED);
}
