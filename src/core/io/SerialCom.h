#pragma once
#include <iostream>
#include <unordered_map>
#include <future>
#include "usb_serial.h"
#include "register.h"

struct SerialParameters
{
	float default_delta;
	int default_precision;
};

/* ESP301 specific control class */
/* This is the mid range calling for each axis/motor, using boost.python later on for larger control */
class SerialCom
{
public:
	SerialCom(std::string port);
	~SerialCom();

	/* Non-control API */
	void readDefaultValues(IO::IniFileData data);

	/* Synchronous API */
	bool turnOn(int axis);
	bool turnOff(int axis);
	bool queryOn(int axis);

	float moveAbsoluteSync(int axis, float target);
	float moveRelativeSync(int axis, float target);

	void waitForStop(int axis);

	/* Asynchronous API */
	std::future<float> moveAbsoluteAsync(int axis, float target);
	std::future<float> moveRelativeAsync(int axis, float target);
	void moveAbsoluteAsyncNoWait(int axis, float target);


	float getAbsoluteSync(int axis);

private:
	HANDLE handle;
	std::unordered_map<std::string, std::promise<float>> promises;

	bool issueCommand(const std::string command, const int axis = 0, const std::string right = "");
	bool executeCommand();
	std::string getResponse();

	SerialParameters params;

	const int precision = 4; //this value will be parsed later from the config file

};
