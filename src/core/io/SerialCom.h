#pragma once
#include <iostream>
#include <unordered_map>
#include <future>
#include "usb_serial.h"
#include "register.h"

/* ESP301 specific control class */
/* This is the mid range calling for each axis/motor, using boost.python later on for larger control */
class SerialCom
{
public:
	SerialCom(std::string port);
	~SerialCom();

	/* Non-control API */
	void readDefaultValues(const std::string config);

	/* Synchronous API */
	bool turnOn(int axis);
	bool turnOff(int axis);
	bool queryOn(int axis);

	float moveAbsoluteSync(int axis, float target);
<<<<<<< HEAD
=======
	std::future<float> moveAbsoluteAsync(int axis, float target);

	void moveAbsoluteAsyncNoWait(int axis, float target);

	void waitForStop(int axis);

>>>>>>> e9e30e2fd8e65ad01a46827d64f8f21470292b50
	float moveRelativeSync(int axis, float target);

	/* Asynchronous API */
	std::future<float> moveAbsoluteAsync(int axis, float target);
	std::future<float> moveRelativeAsync(int axis, float target);

	float getAbsoluteSync(int axis);

private:
	HANDLE handle;
	std::unordered_map<std::string, std::promise<float>> promises;

	bool issueCommand(const std::string command, const int axis = 0, const std::string right = "");
	bool executeCommand();
	std::string getResponse();

	IO::IniFileProperties properties;

	const int precision = 4; //this value will be parsed later from the config file
};

