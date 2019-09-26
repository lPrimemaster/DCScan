#include <iostream>
#include <fstream>
#include <string>
#include "../worker_threads.h"
#include "../base/Timer.h"

#include "../../ctrl/CFlush.h"

#include "../io/usb_serial.h"
#include "../io/register.h"

#include "execution.h"
#include "include/DCScan.h"

#include "../../version.h"

/* This execute from python is clean from all dev branches */
int executeDefault(DCScan_Context* acquisition)
{
	//Initialize default windows handle for operation
	//CFlush::InitHandle();

	//Redirect cerr to file
	std::ofstream filebuffer("logs/" + GET_VERSION_STR() + ".log");
	std::streambuf* cerrbuf = std::cerr.rdbuf();
	std::cerr.rdbuf(filebuffer.rdbuf());

	//Redirect cout to modified printf - FIXME
	std::stringstream outbuffer;
	std::streambuf* outbuf = std::cout.rdbuf();
	std::cout.rdbuf(outbuffer.rdbuf());

	ThreadManager manager;

	char def_path[64];
	strcpy_s(def_path, "config\\");
	strcat_s(def_path, acquisition->filename);

	IO::IniFileData defaultConfig = IO::readIniFile("config\\default.ini");
	IO::IniFileData userConfig = IO::readIniFile(def_path);

	AcquireDataOptions doptions;
	TaskProperties properties;

	properties.name = "task0";
	properties.channel.name = defaultConfig["AcquireSettings"]["default_channel"].c_str();
	properties.channel.assignedChannel = defaultConfig["AcquireSettings"]["default_assigned"].c_str();
	properties.channel.channelUnits = atoi(defaultConfig["AcquireSettings"]["default_units"].c_str());
	properties.channel.customScaleName = "";
	properties.channel.minValue = atof(defaultConfig["AcquireSettings"]["default_min"].c_str());
	properties.channel.maxValue = atof(defaultConfig["AcquireSettings"]["default_max"].c_str());

	properties.timer.activeEdge = DAQmx_Val_Rising;
	properties.timer.sampleMode = DAQmx_Val_ContSamps;
	properties.timer.sampleRate = atof(defaultConfig["AcquireSettings"]["default_timer"].c_str());
	properties.timer.samplesPerChannel = atoi(defaultConfig["AcquireSettings"]["default_spc"].c_str());
	properties.timer.source = "";

	doptions.tproperties = properties;

	FILE* f;
	fopen_s(&f, std::string(defaultConfig["IOLocation"]["relative_path"] + "/" 
		+ defaultConfig["IOLocation"]["name"] + "." 
		+ defaultConfig["IOLocation"]["extension"]).c_str(), "w");
	if (f == nullptr)
	{
		std::cerr << "IO error: could not find/open the specified file: " 
			<< std::string(defaultConfig["IOLocation"]["relative_path"] + "/" 
				+ defaultConfig["IOLocation"]["name"] + "." 
				+ defaultConfig["IOLocation"]["extension"]) << std::endl;
		return -1;
	}
	fprintf(f, "Packet,Point,Data,Timestamp [Software],Timestamp [Hardware][us],Packet Delta [ms - us]\n");

	//The following nomenclature is to be followed
	//Convert all datatype pointers to intptr_t and then pass them to the required function or thread, unwrapping it latter
	auto tid_0 = manager.addThread(acquireThread, &doptions);
	auto tid_1 = manager.addThread(processThread, convertToIntPointer(f, &doptions));

	//CFlush::FlushConsoleStream(&outbuffer);

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	manager.joinThreadSync(tid_0);
	manager.joinThreadSync(tid_1);

	fclose(f);

	//CFlush::ClearConsole(0, CFlush::rows - THREAD_CONCURRENCY);
	//CFlush::FlushConsoleStream(&outbuffer);

	//Always revert the .ini file to default for safety purposes and test only
	IO::createIniFile("config\\default.ini");

	//CFlush::FlushConsoleStream(&outbuffer);

	//Close the handle only when all user threads stopped
	//CFlush::CloseHandle();

	return 0;
}
