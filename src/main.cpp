#include <iostream>
#include <fstream>
#include <string>
#include "core/worker_threads.h"
#include "core/base/Timer.h"
#include "core/base/counter.h"



#define NAME "DCScan"
#define VERSION "2.0-b1"

//Options for later work : César
//Opt 1 - NI-DAQmx intrinsic handshaking for communication with engines
//Opt 2 - Software timing / event for communication with engines

//TODOS : César
//Motors communication / control implementation
//Api implementation for use in external GUI (for : Tiago ?)

//Test counter func
double gaussianFuncSum(double x, int b, double u, double o)
{
	const int N = 20;
	const double sigma = o;
	const double mu = u;

	double fValue = 0.0;

	for (int i = 0; i < N; i++)
	{
		double exponent = ((x - (mu + b * i * 0.1)) / sigma) * ((x - (mu + b * i * 0.1)) / sigma);
		double A = 5.0 * sin(i);
		fValue += (A / (sigma * sqrt(2 * 3.14159264358979))) * exp(-0.5 * exponent);
	}

	return fValue;
}

int main(int argc, char* argv[])
{
	Timestamp ts = Timer::apiTimeSystemHRC();

	std::cout << "Program started... " << std::endl << "Current time - " << ts.year << "/" << ts.month << "/" << ts.day << " " << ts.hour << ":" << ts.min << ":" << ts.sec << ":" << ts.millis << ":" << ts.micros << ":" << ts.nanos << std::endl;

	std::ofstream out(NAME "-" VERSION ".log");
	std::streambuf *cerrbuf = std::cerr.rdbuf();
	std::cerr.rdbuf(out.rdbuf());

	std::cout << "Control thread [0x" << std::hex << std::this_thread::get_id() << "] started." << std::endl;

	ThreadManager manager;

	AcquireDataOptions doptions;

	TaskProperties properties;

	properties.name = "task0";
	properties.channel.name = "dev1/ai0";
	properties.channel.assignedChannel = "";
	properties.channel.channelUnits = DAQmx_Val_Volts;
	properties.channel.customScaleName = "";
	properties.channel.minValue = -10.0;
	properties.channel.maxValue =  10.0;

	properties.timer.activeEdge = DAQmx_Val_Rising;
	properties.timer.sampleMode = DAQmx_Val_ContSamps;
	properties.timer.sampleRate = 10000.0;
	properties.timer.samplesPerChannel = 1000;
	properties.timer.source = "";

	doptions.tproperties = properties;

	FILE* f;
	fopen_s(&f, "data_out.csv", "w");
	fprintf(f, "Packet,Point,Data,Timestamp [Software],Timestamp [Hardware][us],Packet Delta [ms - us]\n");

	auto tid_0 = manager.addThread(acquireThread, &doptions);
	auto tid_1 = manager.addThread(processThread, f);

	auto val = 'r';

	while ( val != 's')
	{
		val = getchar();
	}

	float64 data[1000];

	for (int i = 0; i < 1000; i++)
	{
		double x = i / 100.0;
		data[i] = gaussianFuncSum(x, 16, -8.9, 0.45);
	}

	std::cout << std::dec << std::endl;

	std::cout << "Number of counts in f(x), [0,10] with [m,M] = [1.5,4.4]: ";
	auto tp1 = std::chrono::high_resolution_clock::now();
	auto v1 = Counter::schmittTriggerData(data, 1000, 1.5, 4.4);
	auto diff1 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - tp1).count();
	std::cout << "ST ptime (us): " << diff1 << std::endl; // => about 3-4 us on my machine : César
	std::cout << v1 << std::endl;

	for (int i = 0; i < 1000; i++)
	{
		double x = (i + 1000) / 100.0;
		data[i] = gaussianFuncSum(x, 16, -8.9, 0.45);
	}

	std::cout << "Number of counts in f(x), [10,20] with [m,M] = [1.5,4.4]: ";
	auto tp2 = std::chrono::high_resolution_clock::now();
	auto v2 = Counter::schmittTriggerData(data, 1000, 1.5, 4.4);
	auto diff2 = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - tp2).count();
	std::cout << "ST ptime (us): " << diff2 << std::endl;
	std::cout << v2 << std::endl;


	manager.joinThreadSync(tid_0);
	manager.joinThreadSync(tid_1);

	fclose(f);

	std::cout << "Program uptime: " << Timer::apiUptimeString() << std::endl;
	return 0;
}