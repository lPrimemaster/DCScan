#include "worker_threads.h"
#include "worker_callbacks.h"
#include "base/counter.h"
#include "base/Timer.h"
#include "memory/DataChunk.h"
#include "../ctrl/CFlush.h"
#include "../ctrl/PyScript.h"
#include "../core/io/register.h"
#include <chrono>

#pragma warning( disable : 26444 )

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
	auto [f, opt, script] = from_intPointer(3, FILE*, AcquireDataOptions*, PyScript*)(reinterpret_cast<intptr_t*>(data));
	int x = 0;
	while (true)
	{
		while (CallbackPacket::getGlobalCBPStack()->empty() && flags->load() == THREAD_RUN)
		{
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

		//TODO: Check use cases
		/*static DataChunk<uInt32,    1> dd_mem_chunk(dpacket.data_size, DEFAULT_DATA);
		static DataChunk<long long, 1> td_mem_chunk(dpacket.data_size,    TIME_DATA);*/

		//Process the data
		static const long long dt = 1000000000 / opt->tproperties.timer.sampleRate; //In nanoseconds
		static int dpser = 0;

		long long ns = dpacket.software_tor_ns;
		std::vector<long long> local_ns_table;
		local_ns_table.reserve(dpacket.data_size);

		//No problem with const static lambda here as the data_size should not vary from the first packet size
		const static auto getLocal_ns = [ns, dpacket](size_t i) -> long long { return ns - dt * (dpacket.data_size - i); };

		if (dpacket.data != nullptr)
		{
			//FIX: THIS CAN BE OMMITED
			for (int i = 0; i < dpacket.data_size; i++)
			{
				long long local_ns = getLocal_ns(i);
				Timestamp ts = Timer::apiTimeSystemHRC_NanoToTimestamp(local_ns);
				local_ns_table.push_back(local_ns);

				fprintf(f, "%d,%d,%d,%lf,%s\n", dpser, i, 1000 * dpser + i, dpacket.data[i], Timer::timeStampToString(ts).c_str());
			}
			dpser++;

			//Noise threshold is equal bilateraly - 1.0 => TODO: Make this unilateral specific
			//TODO: Normalize counts per unit time
			//TODO: Get count width to analyse possible stacked data (pile up)
			//TODO: Calculate peak time
			//FIX: Timing values are calculated as soon as the rising edge is triggered - Data peaks are probably very symmetric!!
			//DONE: Calculate count timing according to place where it was detected
			auto [count, places] = Counter::countPacket(dpacket.data, dpacket.data_size, 1.0, 0.5);
			std::vector<long long> inplace_ns;
			std::vector<Timestamp> inplace_ts;
			inplace_ns.reserve(places.size());
			inplace_ts.reserve(places.size());

			std::for_each(places.begin(), places.end(), [&](const size_t& val) { inplace_ns.push_back(getLocal_ns(val)); });
			std::for_each(inplace_ns.begin(), inplace_ns.end(), [&](const long long& val) { inplace_ts.push_back(Timer::apiTimeSystemHRC_NanoToTimestamp(val)); });
			
			//FIX: This loses the time lost between datapacket interchange
			auto delta_ns = count != 0 ? *(inplace_ns.end() - 1) - *inplace_ns.begin() : 0;

			try
			{
				if (CallBackRegistries::data_count_callback.ptr() && script->getAtomicState())
				{
					py::gil_scoped_acquire acquire;
					CallBackRegistries::data_count_callback(count, x++ /* ANGLE WILL BE HERE */, delta_ns);
					py::gil_scoped_release release;
				}
			}
			catch (py::error_already_set& eas)
			{
				std::cerr << eas.what() << std::endl;
			}

			//Free data copy
			free(dpacket.data);

		}
	}

	flags->store(THREAD_ENDED);
}

void controlThread(std::atomic<int>* flags, void* data)
{
	auto [IniData, script] = from_intPointer(2, IO::IniFileData*, PyScript*)(reinterpret_cast<intptr_t*>(data));

	int x = 0;

	while (true)
	{
		if (flags->load() == THREAD_HALT)
			break;

		Timestamp ts = Timer::apiTimeSystemHRC();
		CFlush::println(1, "Current Time: %s", CFlush::formatString("%02d:%02lld:%02lld", ts.hour, ts.min, ts.sec).c_str());
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

#if 0
		try
		{
			//If script is not started this will skip
			//TODO: Move out of this thread - only here for testing
			if (CallBackRegistries::data_count_callback.ptr() && script->getAtomicState())
			{
				py::gil_scoped_acquire acquire;
				CallBackRegistries::data_count_callback(10 * std::sin(x++ / 10.0), x, 0.1);
				py::gil_scoped_release release;
			}
			if (x > 100) x = 0;
		}
		catch (py::error_already_set & eas)
		{
			std::cerr << eas.what() << std::endl;
		}
#endif
	}
	flags->store(THREAD_ENDED);
}

void userGUIThread(std::atomic<int>* flags, void* data)
{
	auto [script] = from_intPointer(1, PyScript*)(reinterpret_cast<intptr_t*>(data));

	PyScript::InitInterpreter();

	script->operator()();

	PyScript::DestInterpreter();

	flags->store(THREAD_ENDED);
}
