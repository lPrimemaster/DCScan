#pragma once
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <atomic>

#define NOMINMAX

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <pybind11/numpy.h>

#include "ThreadManager.h"

class PyFunc;

namespace py = pybind11;

class PyScript
{
public:
	PyScript(std::filesystem::path filepath);
	~PyScript();

	bool operator()();

	bool setWorkingDir(std::filesystem::path dir = "scripts");

	static void InitInterpreter();
	static void DestInterpreter();
	inline int getAtomicState()
	{
		return state.load();
	}

	static inline py::dict& getInterpreterLocals()
	{
		return locals;
	}

	static inline py::dict& getInterpreterGlobals()
	{
		return globals;
	}

private:
	py::module module;
	char* script_body = nullptr;
	std::filesystem::path wdir = "scripts";
	std::wstring s_name;

	static inline py::dict locals;
	static inline py::dict globals;

	std::atomic<int> state;
	static py::module sys;
	static bool intRunning;
};

