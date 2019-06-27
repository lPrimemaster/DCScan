#include "SerialCom.h"
#include <string>
#include <algorithm>
#include <typeinfo>
#include <array>
#include "../base/common.h"

#define STRFY(x) #x

#define INSERT_VALUE(value) typeid atof(dvValues[STRFY(value)].c_str());

SerialCom::SerialCom(std::string port)
{
	//Init with default settings for now - No timeouts set yes =>  All in accordance with Newport's "documentation"
	SerialArgs args;
	args.baudRate = 921600;		//921.6 kBd
	args.byteSize = 8;			//8 bit size
	args.eofChar = '\r';		//Carriage return command eof (?)
	args.parity = NOPARITY;		//No parity
	args.stopBits = ONESTOPBIT;	//One stop bit

	handle = serial_initHandle(port.c_str(), GENERIC_READ | GENERIC_WRITE, args);
}


SerialCom::~SerialCom()
{
	serial_closeHandle(handle);
}


bool SerialCom::loadConfig(IO::IniFileData data)
{
	auto iofp = IO::IniFileProperties();
	auto values = iofp.sub_sec.at("ControlSettings");

	const std::array<std::array<std::string, 2>, 50> orderedCommands = { {
		{ values.at(0) , "??" },
		{ values.at(1) , "QM" },
		{ values.at(2) , "SN" },
		{ values.at(3) , "FP" },
		{ values.at(4) , "ZB" },
		{ values.at(5) , "ZA" },
		{ values.at(6) , "ZE" },
		{ values.at(7) , "ZF" },
		{ values.at(8) , "ZH" },
		{ values.at(9) , "ZS" },
		{ values.at(10), "SU" },
		{ values.at(11), "FR" },
		{ values.at(12), "QS" },
		{ values.at(13), "QV" },
		{ values.at(14), "QI" },
		{ values.at(15), "QG" },
		{ values.at(16), "QT" },
		{ values.at(17), "SL" },
		{ values.at(18), "SR" },
		{ values.at(19), "TJ" },
		{ values.at(20), "OM" },
		{ values.at(21), "VU" },
		{ values.at(22), "VA" },
		{ values.at(23), "JH" },
		{ values.at(24), "JW" },
		{ values.at(25), "OH" },
		{ values.at(26), "VB" },
		{ values.at(27), "AU" },
		{ values.at(28), "AC" },
		{ values.at(29), "AG" },
		{ values.at(30), "AE" },
		{ values.at(31), "JK" },
		{ values.at(32), "KP" },
		{ values.at(33), "KI" },
		{ values.at(34), "KD" },
		{ values.at(35), "VF" },
		{ values.at(36), "AF" },
		{ values.at(37), "KS" },
		{ values.at(38), "FE" },
		{ values.at(39), "DB" },
		{ values.at(40), "CL" },
		{ values.at(41), "QR" },
		{ values.at(42), "QR" },
		{ values.at(43), "SS" },
		{ values.at(44), "GR" },
		{ values.at(45), "SI" },
		{ values.at(46), "SK" },
		{ values.at(47), "SK" },
		{ values.at(48), "BA" },
		{ values.at(49), "CO" }
	} };

	//Avoid ESP-301 buffer overflow with commands in the setup moment by waiting for the response of the controller
	int k = 0;
	for (auto mc : orderedCommands)
	{
		auto str = data["ControlSettings"][mc[0]];
		auto value = IO::convertBracketValue<std::string>(str);
#error fix -> convertBracketValue()
		for (int i = 0; i < 3; i++)
		{
			issueCommand(mc[1], i, value.at(i));
		}

		executeCommand();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		for (int i = 0; i < 3; i++)
		{
			/*issueCommand(mc.second, i, "?");
			executeCommand();*/
			//fprintf(stderr, "Read config %s = %s\n", mc.second.c_str(), getResponse().c_str());
		}
	}
	return true;
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

bool SerialCom::queryState(int axis)
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
