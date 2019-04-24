#pragma once
#include <iostream>
#include <unordered_map>
#include <future>
#include "usb_serial.h"

/* ESP301 specific control class */
/* This is the mid range calling for each axis/motor, using boost.python for larger control */
class SerialCom
{
public:
	SerialCom(std::string port);
	~SerialCom();

	bool turnOn(int axis);
	bool turnOff(int axis);
	bool queryOn(int axis);

	float moveAbsoluteSync(int axis, float target);
	std::future<float> moveAbsoluteAsync(int axis, float target);

	float moveRelativeSync(int axis, float target);
	std::future<float> moveRelativeAsync(int axis, float target);

private:
	HANDLE handle;
	std::unordered_map<std::string, std::promise<float>> promises;

	bool issueCommand(const std::string command, const int axis = 0, const std::string right = "");
	bool executeCommand();
	std::string getResponse();

	const int precision = 4;
};

