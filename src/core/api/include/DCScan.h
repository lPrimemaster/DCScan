#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "../../worker_threads.h"
#include "../../base/Timer.h"
#include "../../base/counter.h"

#include "../../../ctrl/CFlush.h"

#include "../../io/usb_serial.h"
#include "../../io/register.h"

#include "../../../version.h"

#include "../../io/SerialCom.h"

#include "../../../ctrl/PerfCount.h" //Test only
#include "../../../ctrl/PyScript.h"  //Test only

//Define EMBEDED PYMODULES for export
namespace py = pybind11;

//All the modules only work in a embedded interpreter

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
		.def("__repr__", 
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