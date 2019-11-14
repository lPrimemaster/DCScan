#include "PyScript.h"
#include "../core/worker_callbacks.h"

bool PyScript::intRunning = false;
py::module PyScript::sys;

PyScript::PyScript(std::filesystem::path filepath)
{
	//Python sys.argv supports wchar_t*
	s_name = filepath.filename().native();

	auto path = wdir / filepath;
	if (path.extension() != ".py")
	{
		std::cerr << "Can't run python script with wrong extension - " << path.extension() << std::endl;
		return;
	}

	std::ifstream file(path.native());

	if (!file.is_open() || !file.good())
	{
		std::cerr << "Script: " << filepath << " couldn't be opened." << std::endl;
		return;
	}

	std::stringstream ss;
	std::string line;

	while (std::getline(file, line))
	{
		ss << line << '\n';
	}

	file.close();

	script_body = new char[ss.str().size() + 1];

	std::strcpy(script_body, ss.str().c_str());
}

PyScript::~PyScript()
{
	if (script_body != nullptr)
	{
		delete[] script_body;
	}
}

bool PyScript::operator()()
{
	//Execute the script
	if (script_body == nullptr)
	{
		std::cerr << "Script body is empty. Can't run. Aborting." << std::endl;
		return false;
	}

	if (PyScript::intRunning)
	{
		bool status = true;
		state.store(1);
		try
		{
			py::exec(script_body, py::globals(), py::object());
		}
		catch(py::error_already_set & eas)
		{
			std::cerr << eas.what() << std::endl;
			status = false;
		}
		state.store(0);
		return status;
	}

	std::cerr << "Python interpreter is not running. Aborting." << std::endl;
	return false;
}

bool PyScript::setWorkingDir(std::filesystem::path dir)
{
	if (std::filesystem::exists(dir))
	{
		wdir = dir;
		return true;
	}

	std::cerr << "Directory " << dir << "doesn't exist. Aborting." << std::endl;
	return false;
}

void PyScript::InitInterpreter()
{
	py::initialize_interpreter();
	intRunning = true;
}

void PyScript::DestInterpreter()
{
	py::finalize_interpreter();
	intRunning = false;
}
