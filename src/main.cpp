#include <iostream>
#include <fstream>
#include <string>
#include "core/worker_threads.h"
#include "core/base/Timer.h"
#include "core/base/counter.h"

#include "ctrl/CFlush.h"

#include "core/io/usb_serial.h"
#include "core/io/register.h"

#include "version.h"

#include "core/io/SerialCom.h"

//Options for later work : César
//Opt 1 - NI-DAQmx intrinsic handshaking for communication with engines
//Opt 2 - Software timing / event for communication with engines

//TODOS : César
//Motors communication / control implementation
//Api implementation for use in external GUI (for : Tiago ?)
//Fix an error with thread concurrency at ThreadManager.cpp : 67 -> pool erased
//Create a system where there can be more than one .ini file, for multiple configurations
//Fix directory non existent for data or config files

int main(int argc, char* argv[])
{
	//Initialize default windows handle for operation
	CFlush::Init();

	//Redirect cerr and stderr to file
	FILE* nstderr = NULL;
	freopen_s(&nstderr, std::string("logs/" + GET_VERSION_STR() + ".log").c_str(), "w", stderr);

	//Redirect cout to modified print - FIXME
	std::stringstream outbuffer;
	std::streambuf *outbuf = std::cout.rdbuf();
	//std::cout.rdbuf(outbuffer.rdbuf());

	ThreadManager manager;

	Timestamp ts = Timer::apiTimeSystemHRC();

	CFlush::println(0, "Program started!");
	CFlush::println(1, "Current Time: %s", CFlush::formatString("%02d:%02d:%02d", ts.hour, ts.min, ts.sec).c_str());
	/*std::cout << "Main thread [0x" << std::hex << std::uppercase << std::this_thread::get_id() << std::nouppercase << "] started." << std::dec << std::endl;*/

	//IO::createIniFile("config\\default.ini");
	IO::IniFileData data = IO::readIniFile("config\\default.ini");

	AcquireDataOptions doptions;
	TaskProperties properties;

	properties.name = "task0";
	properties.channel.name = data["AcquireSettings"]["default_channel"].c_str();
	properties.channel.assignedChannel = data["AcquireSettings"]["default_assigned"].c_str();
	properties.channel.channelUnits = atoi(data["AcquireSettings"]["default_units"].c_str());
	properties.channel.customScaleName = "";
	properties.channel.minValue = atof(data["AcquireSettings"]["default_min"].c_str());
	properties.channel.maxValue = atof(data["AcquireSettings"]["default_max"].c_str());

	properties.timer.activeEdge = DAQmx_Val_Rising;
	properties.timer.sampleMode = DAQmx_Val_ContSamps;
	properties.timer.sampleRate = atof(data["AcquireSettings"]["default_timer"].c_str());
	properties.timer.samplesPerChannel = atoi(data["AcquireSettings"]["default_spc"].c_str());
	properties.timer.source = "";

	doptions.tproperties = properties;

	FILE* f;
	fopen_s(&f, std::string(data["IOLocation"]["relative_path"] + "/" 
		+ data["IOLocation"]["name"] + "." 
		+ data["IOLocation"]["extension"]).c_str(), "w");
	if (f == nullptr)
	{
		std::cerr << "IO error: could not find/open the specified file: " 
			<< std::string(data["IOLocation"]["relative_path"] + "/" 
				+ data["IOLocation"]["name"] + "." 
				+ data["IOLocation"]["extension"]) << std::endl;
		return -1;
	}
	fprintf(f, "Packet,Point,Data,Timestamp,\n");

	//The following nomenclature is to be followed
	//Convert all datatype pointers to intptr_t and then pass them to the required function or thread, unwrapping it later

	auto tid_0 = manager.addThread(acquireThread, &doptions);
	auto tid_1 = manager.addThread(processThread, convertToIntPointer(f, &doptions));
	auto tid_2 = manager.addThread(controlThread, convertToIntPointer(&data));

	//CFlush::FlushConsoleStream(&outbuffer);

	std::this_thread::sleep_for(std::chrono::milliseconds(10000));

	manager.joinThreadSync(tid_0);

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	manager.joinThreadSync(tid_1);

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	manager.joinThreadSync(tid_2);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	fclose(f);

	//CFlush::FlushConsoleStream(&outbuffer);

	CFlush::println(2, "Program uptime: %s", Timer::apiUptimeString().c_str());

	//CFlush::ClearConsole(0, CFlush::rows - THREAD_CONCURRENCY);
	//CFlush::FlushConsoleStream(&outbuffer);

	//Always revert the .ini file to default for safety purposes and test only
	IO::createIniFile("config\\default.ini");

	//Close the handle only when all user threads stopped
	//CFlush::CloseHandle();

	fclose(nstderr);

	return 0;
}