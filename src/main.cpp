#include <iostream>
#include <fstream>
#include <string>
#include "core/worker_threads.h"
#include "core/base/Timer.h"
#include "core/base/counter.h"

#include "core/io/usb_serial.h"

#define NAME "DCScan"
#define VERSION "2.0-b1"

//Options for later work : César
//Opt 1 - NI-DAQmx intrinsic handshaking for communication with engines
//Opt 2 - Software timing / event for communication with engines

//TODOS : César
//Motors communication / control implementation
//Api implementation for use in external GUI (for : Tiago ?)

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

	manager.joinThreadSync(tid_0);
	manager.joinThreadSync(tid_1);

	fclose(f);

	libusb_context ** ctx = NULL;

	usb_serial_init_libusb(ctx, 0);
	usb_serial_list_devices(ctx);
	usb_serial_deinit_libusb(ctx);

	std::cout << "Program uptime: " << Timer::apiUptimeString() << std::endl;
	return 0;
}