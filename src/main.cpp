#include <iostream>
#include <fstream>
#include <string>
#include <io.h>
#include <fcntl.h>

//Main core behaviour
#include "core/worker_threads.h"

//Basic functionalities exposed
#include "core/base/Timer.h"
#include "core/base/counter.h"

//Core overloaded printf (backend <-> frontend)
#include "core/utils/OLstreambuf.h"

//Core io behaviour
#include "core/io/usb_serial.h"
#include "core/io/register.h"
#include "core/io/SerialCom.h"

//Data management
#include "core/memory/DataChunk.h" //Test only

//Pipeline / frontend & stdout(err) control management 
#include "ctrl/CFlush.h"
#include "ctrl/PyScript.h"
#include "ctrl/PerfCount.h"

//Custom versioning
#include "version.h"


//Options for later work : César
//Opt 1 - NI-DAQmx intrinsic handshaking for communication with engines -- DONE
//Opt 2 - Software timing / event for communication with engines -- DONE

//TODO : César
//Motors communication / control implementation -- DONE
//Api implementation for use in external GUI -- DONE
//Fix an error with thread concurrency at ThreadManager.cpp : 67 -> pool erased -- DONE
//Use PyQt for the front-end -- DONE
//Create a system where there can be more than one .ini file, for multiple configurations
//Fix directory non existent for data or config files
//Create a sized buffer on the screen with autoscroll to present std::cout
//Fix warnings
//Create a requirements.txt file for python dependencies
//External Events readout
//Set Engine limits (angle limit)
//Normalize angle for dict -> real angle value in axis

int main(int argc, char* argv[])
{
	//Initialize default windows handle for operation
	CFlush::Init();

	PyScript::setWorkingDir("scripts/gui");
	PyScript script("mw.py");

	//Redirect cerr and stderr to file
	FILE* nstderr = NULL;
	const std::string VERSION = GET_VERSION_STR();
	freopen_s(&nstderr, std::string("logs/" + VERSION + ".log").c_str(), "w", stderr);
	if (nstderr == nullptr)
	{
		std::cerr << "IO error: could not find/open the specified file: " << "logs/" + VERSION + ".log" << std::endl;
		return -1;
	}

	//Redirect cout to window buffer (WinAPI)
	OLstreambuf ols;
	std::cout.rdbuf(&ols);

	//Redirect stdout to window buffer (WinAPI)
	int fd = _open_osfhandle((intptr_t)CFlush::getDefaultHandle(), O_WRONLY | O_TEXT);
	int val = _dup2(fd, _fileno(stdout));

	ThreadManager manager;

	Timestamp ts = Timer::apiTimeSystemHRC();

	CFlush::println("Program started!");
	CFlush::println("Current Time: %s", CFlush::formatString("%02d:%02d:%02d", ts.hour, ts.min, ts.sec).c_str());

	IO::IniFileData data = IO::readIniFile("config\\default.ini");

	AcquireDataOptions doptions;

	doptions.tproperties.name = "task0";
	doptions.tproperties.channel.name = data["AcquireSettings"]["default_channel"].c_str();
	doptions.tproperties.channel.assignedChannel = data["AcquireSettings"]["default_assigned"].c_str();
	doptions.tproperties.channel.channelUnits = atoi(data["AcquireSettings"]["default_units"].c_str());
	doptions.tproperties.channel.customScaleName = "";
	doptions.tproperties.channel.minValue = atof(data["AcquireSettings"]["default_min"].c_str());
	doptions.tproperties.channel.maxValue = atof(data["AcquireSettings"]["default_max"].c_str());

	doptions.tproperties.timer.activeEdge = DAQmx_Val_Rising;
	doptions.tproperties.timer.sampleMode = DAQmx_Val_ContSamps;
	doptions.tproperties.timer.sampleRate = atof(data["AcquireSettings"]["default_timer"].c_str());
	doptions.tproperties.timer.samplesPerChannel = atoi(data["AcquireSettings"]["default_spc"].c_str());
	doptions.tproperties.timer.source = "";

	FILE* f;
	std::string file_loc = std::string(data["IOLocation"]["relative_path"] + "/" + data["IOLocation"]["name"] + "." + data["IOLocation"]["extension"]);
	fopen_s(&f, file_loc.c_str(), "w");
	if (f == nullptr)
	{
		std::cerr << "IO error: could not find/open the specified file: " << file_loc << std::endl;
		return -1;
	}
	fprintf(f, "Packet,Point,Data,Timestamp,\n");

	//The following nomenclature is to be followed
	//Convert all datatype pointers to intptr_t and then pass them to the required function or thread, unwrapping it later

	auto tid_0 = manager.addThread(acquireThread, &doptions);
	auto tid_1 = manager.addThread(processThread, convertToIntPointer(f, &doptions, &script));
	auto tid_2 = manager.addThread(controlThread, convertToIntPointer(&data, &script));
	auto tid_3 = manager.addThread(userGUIThread, convertToIntPointer(&script));

	//PerfCount::PrintValidProcTimes();
	PerfCount::Init();
	//PerfCount::AddCounter(L"\\Process(DCScan)\\% Processor Time");
	PerfCount::AddCounter(L"\\Processor(_Total)\\% Processor Time", PerfCount::CounterID::PROCESSOR_TIME);
	//PerfCount::AddCounter(L"\\Memory\\Write Copies/sec", PerfCount::CounterID::MEMCOPIES_SEC);

	auto tid_4 = manager.addThread(PerfCount::Record, nullptr);

	std::this_thread::sleep_for(std::chrono::seconds(600));

	manager.joinThreadSync(tid_0);

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	manager.joinThreadSync(tid_1);

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	manager.joinThreadSync(tid_2);

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	manager.joinThreadSync(tid_3);

	manager.joinThreadSync(tid_4);

	fclose(f);

	CFlush::println(2, "Program uptime: %s", Timer::apiUptimeString().c_str());

	//Always revert the .ini file to default for safety purposes and test only
	IO::createIniFile("config\\default.ini");

	fclose(nstderr);
	_close(fd);

	return 0;
}