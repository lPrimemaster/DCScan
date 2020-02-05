#pragma once
#include <iostream>
#include <fstream>
#include <string>

#define NOMINMAX

#include <pybind11/iostream.h>

#include "../../worker_threads.h"
#include "../../worker_callbacks.h"
#include "../../base/Timer.h"
#include "../../base/counter.h"

#include "../../../ctrl/CFlush.h"

#include "../../io/usb_serial.h"
#include "../../io/register.h"

#include "../../../version.h"

#include "../../io/SerialCom.h"

#include "../../../ctrl/PerfCount.h"
#include "../../../ctrl/PyScript.h"

#include "../../../core/memory/DataChunk.h"

//Define EMBEDED PYMODULES for export
namespace py = pybind11;

//All the modules only work in a embedded interpreter

//DCS Modules

//Common module
PYBIND11_EMBEDDED_MODULE(DCS_Common, m)
{
	//Overload python print
	m.def("print", [](py::object& str) { SetConsoleCursorPosition(CFlush::getDefaultHandle(), { 0, CFlush::current_ypos++ }); py::print(str); });
}

//Timing helper functions
PYBIND11_EMBEDDED_MODULE(DCS_Time, m)
{
	py::class_<Timestamp>(m, "Timestamp")
		.def_readwrite("year", &Timestamp::year)
		.def_readwrite("month", &Timestamp::month)
		.def_readwrite("day", &Timestamp::day)
		.def_readwrite("hour", &Timestamp::hour)
		.def_readwrite("min", &Timestamp::min)
		.def_readwrite("sec", &Timestamp::sec)
		.def_readwrite("millis", &Timestamp::millis)
		.def_readwrite("micros", &Timestamp::micros)
		.def_readwrite("nanos", &Timestamp::nanos)
		.def("__repr__", //TODO: Format this to be fancy
			[](const Timestamp& ts)
				{
					return "<Timestamp : '" + std::to_string(ts.hour) + ":" + std::to_string(ts.min) + 
						":" + std::to_string(ts.sec) + ":" + std::to_string(ts.millis) + 
						"." + std::to_string(ts.micros) + "." + std::to_string(ts.nanos) + "'>";
				}
			);

	m.def("systemHRC", &Timer::apiTimeSystemHRC, py::return_value_policy::copy);
	m.def("systemHRC_Nano", &Timer::apiTimeSystemHRC_Nano, py::return_value_policy::copy);
	m.def("systemHRC_Nano2Timestamp", &Timer::apiTimeSystemHRC_NanoToTimestamp, py::return_value_policy::copy);
	m.def("systemHRC_Delta2Timestamp", &Timer::apiTimeSystemHRC_DeltaToTimestamp, py::return_value_policy::copy);

	m.def("uptimeString", &Timer::apiUptimeString, py::return_value_policy::copy);
	m.def("systemClockString", &Timer::apiTimeSystemClockString, py::return_value_policy::copy);
}

//Data request module
//This module creates a copy out of the required number of Datapackets wrapped arround python compatible types, to use on the front end
//All the sent values are copies of the originals, since they are going to be used later on
//The problem is on filling the memory, so counts would be a moroe viable option ??
//TODO
PYBIND11_EMBEDDED_MODULE(DCS_Data, m)
{
	m.def("lastPacket",		  &getBEMC<uInt32,    1, DC_Data::DEFAULT_DATA>);
	m.def("lastPacketTimeNS", &getBEMC<long long, 1,    DC_Data::TIME_DATA>);

	m.def("registerDataCallback", &registerDataCounter);
}

//Backend Control Module
PYBIND11_EMBEDDED_MODULE(DCS_Control, m)
{
	m.def("setServerAcquireState", &setAcquisitionState);
}
