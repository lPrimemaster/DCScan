#include <iostream>
#include <fstream>
#include <string>
#include "core/worker_threads.h"
#include "core/base/Timer.h"
#include "core/base/counter.h"

#include "ctrl/CFlush.h"

#include "core/io/usb_serial.h"
#include "core/io/register.h"

#define NAME "DCScan"
#define VERSION "2.0-b3"

//Options for later work : César
//Opt 1 - NI-DAQmx intrinsic handshaking for communication with engines
//Opt 2 - Software timing / event for communication with engines

//TODOS : César
//Motors communication / control implementation
//Api implementation for use in external GUI (for : Tiago ?)
//Fix an error with thread concurrency at ThreadManager.cpp : 67 -> pool erased

int main(int argc, char* argv[])
{
	//Initialize default windows handle for operation
	CFlush::InitHandle();

	//Redirect cerr to file
	std::ofstream filebuffer(NAME "-" VERSION ".log");
	std::streambuf *cerrbuf = std::cerr.rdbuf();
	std::cerr.rdbuf(filebuffer.rdbuf());

	//Redirect cout to modified printf - FIXME
	std::stringstream outbuffer;
	std::streambuf *outbuf = std::cout.rdbuf();
	std::cout.rdbuf(outbuffer.rdbuf());

	ThreadManager manager;

	Timestamp ts = Timer::apiTimeSystemHRC();

	std::cout << "Program started... " << std::endl << "Current time - " << ts.year << "/" << ts.month << "/" << ts.day << " " << ts.hour << ":" << ts.min << ":" << ts.sec << ":" << ts.millis << ":" << ts.micros << ":" << ts.nanos << std::endl;
	std::cout << "Main thread [0x" << std::hex << std::this_thread::get_id() << "] started." << std::endl;

	IO::IniFileData data = IO::readIniFile("config\\dcscan.ini");

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
	properties.timer.sampleRate = atof(data["ACQSETTINGS"]["DEFAULT_TIMER"].c_str());
	properties.timer.samplesPerChannel = 1000;
	properties.timer.source = "";

	doptions.tproperties = properties;

	FILE* f;
	fopen_s(&f, std::string(data["IOLOC"]["RELATIVE_PATH"] + "/" + data["IOLOC"]["NAME"] + "." + data["IOLOC"]["EXTENSION"]).c_str(), "w");
	fprintf(f, "Packet,Point,Data,Timestamp [Software],Timestamp [Hardware][us],Packet Delta [ms - us]\n");

	//The following nomenclature is to be followed
	//Convert all datatype pointers to intptr_t and then pass them to the required function or thread, unwrapping it latter

	intptr_t int_ptr[2] = {reinterpret_cast<intptr_t>(f), reinterpret_cast<intptr_t>(&doptions)};

	auto tid_0 = manager.addThread(acquireThread, &doptions);
	auto tid_1 = manager.addThread(processThread, int_ptr);
	//auto tid_2 = manager.addThread(controlThread, NULL);

	CFlush::FlushConsoleStream(&outbuffer);

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	manager.joinThreadSync(tid_0);
	manager.joinThreadSync(tid_1);
	//manager.joinThreadSync(tid_2);

	fclose(f);

	libusb_context ** ctx = NULL;

	usb_serial_init_libusb(ctx, 0);
	usb_serial_device_list lst;
	usb_serial_get_device_list(ctx, &lst);
	usb_serial_print_device_list(&lst);

	//lst works here for access only

	ssize_t s = usb_serial_get_device_index_from_id(&lst, 0x0717B);

	std::cout << std::dec << "Index of found device [by id]: " << s << std::endl;

	s = usb_serial_get_device_index_from_name(&lst, "USB Optical Mouse");

	std::cout << std::dec << "Index of found device [by name]: " << s << std::endl;

	usb_serial_free_device_list(&lst);
	usb_serial_deinit_libusb(ctx);

	std::cout << "Program uptime: " << Timer::apiUptimeString() << std::endl;

	CFlush::ClearConsole(0, CFlush::rows - THREAD_CONCURRENCY);
	CFlush::FlushConsoleStream(&outbuffer);

	//Always revert the .ini file to default for safety purposes and test only
	IO::createIniFile("config\\dcscan.ini");

	CFlush::FlushConsoleStream(&outbuffer);

	//Close the handle only when all user threads stopped
	CFlush::CloseHandle();

	return 0;
}