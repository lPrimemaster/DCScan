#include "SerialCom.h"
#include <string>
#include "../base/common.h"


SerialCom::SerialCom(std::string port)
{
	//Init with default settings for now - No timeouts set yes =>  All in accordance with Newport's "documentation"
	SerialArgs args;
	args.baudRate = 921600;		//921.6 kBd
	args.byteSize = 8;			//8 bit size
	args.parity = NOPARITY;		//No parity
	args.stopBits = ONESTOPBIT;	//One stop bit

	handle = serial_initHandle(port.c_str(), GENERIC_READ | GENERIC_WRITE, args);
}


SerialCom::~SerialCom()
{
	serial_closeHandle(handle);
}

bool SerialCom::turnOn(int axis)
{
	issueCommand("MO", axis);
	issueCommand("MO", axis, "?");
	executeCommand();
	std::string rx = getResponse();
	return rx == "1";
}

bool SerialCom::turnOff(int axis)
{
	issueCommand("MF", axis);
	issueCommand("MF", axis, "?");
	executeCommand();
	std::string rx = getResponse();
	return rx == "0";
}

bool SerialCom::queryOn(int axis)
{
	issueCommand("MO", axis, "?");
	executeCommand();
	std::string rx = getResponse();
	return rx == "1";
}

float SerialCom::moveAbsoluteSync(int axis, float target)
{
	issueCommand("PA", axis, tostring_prec(target, precision));
	issueCommand("WS", axis);
	issueCommand("TP", axis, "?");
	executeCommand();
	float rx = atof(getResponse().c_str());
	return rx;
}

std::future<float> SerialCom::moveAbsoluteAsync(int axis, float target)
{
	std::future<float> future = std::async(std::launch::async, [=]
		{
			issueCommand("PA", axis, tostring_prec(target, precision));
			issueCommand("WS", axis);
			issueCommand("TP", axis, "?");
			executeCommand();
			float rx = atof(getResponse().c_str());
			return rx;
		}
	);
	return future;
}

void SerialCom::moveAbsoluteAsyncNoWait(int axis, float target)
{
	issueCommand("PA", axis, tostring_prec(target, precision));
	executeCommand();
}

void SerialCom::waitForStop(int axis)
{
	issueCommand("WS", axis);
	executeCommand();
}

float SerialCom::moveRelativeSync(int axis, float target)
{
	issueCommand("PR", axis, tostring_prec(target, precision));
	issueCommand("WS", axis);
	issueCommand("TP", axis, "?");
	executeCommand();
	float rx = atof(getResponse().c_str());
	return rx;
}

std::future<float> SerialCom::moveRelativeAsync(int axis, float target)
{
	std::future<float> future = std::async(std::launch::async, [=]
		{
			issueCommand("PR", axis, tostring_prec(target, precision));
			issueCommand("WS", axis);
			issueCommand("TP", axis, "?");
			executeCommand();
			float rx = atof(getResponse().c_str());
			return rx;
		}
	);
	return future;
}

float SerialCom::getAbsoluteSync(int axis)
{
	issueCommand("TP", axis, "?");
	executeCommand();
	float rx = atof(getResponse().c_str());
	return rx;
}

bool SerialCom::issueCommand(const std::string command, const int axis, const std::string right)
{
	const std::string value = axis ? std::to_string(axis) + command + right + ';' : command + right + ';';
	const char* data = value.c_str();
	return serial_writeBytes(handle, data, value.size());
}

bool SerialCom::executeCommand()
{
	//This method works because of the following documentation specification => "Until a terminator is received, characters are
	//simply kept in contiguous buffer space without evaluation."
	return serial_writeBytes(handle, "\r", 2);
}

std::string SerialCom::getResponse()
{
	char response[256];
	DWORD rbSize = 0;
	bool val = serial_readBytes(handle, response, 256, &rbSize);
	return std::string(response);
}
