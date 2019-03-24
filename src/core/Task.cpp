#include "Task.h"
#include <NIDAQmx.h>
#include "base/common.h"
#include "base/Timer.h"

Task::Task(const std::string& taskname) : name(taskname)
{
	int32 error = DAQmxCreateTask(taskname.c_str(), &handle);
	if (!handleError(error, "Task()"))
	{
		std::cerr << "Initializing task " << taskname << std::endl;
	}
}

Task::~Task()
{
	if (handle != nullptr)
	{
		DAQmxStopTask(handle);
		DAQmxClearTask(handle);
		std::cerr << "Ending task " << name << std::endl;
	}
}

void Task::start() const
{
	int32 error = DAQmxStartTask(handle);
	if (!handleError(error, "Task::start()"))
	{
		std::cerr << "Starting task " << name << std::endl;
	}
}

void Task::stop() const
{
	int32 error = DAQmxStopTask(handle);
	if (!handleError(error, "Task::stop()"))
	{
		std::cerr << "Stopping task " << name << std::endl;
	}
}

void Task::addChannel(ChannelType type, TaskProperties::Channel properties)
{
	int32 error = 0;
	switch (type)
	{
	case Task::VoltageChannel:
		error = DAQmxCreateAIVoltageChan(handle, properties.name.c_str(), properties.assignedChannel.c_str(), DAQmx_Val_Cfg_Default,
			properties.minValue, properties.maxValue, properties.channelUnits, properties.customScaleName.c_str());
		if (!handleError(error, "Task::addChannel() - VoltageChannel"))
		{
			std::cerr << "Task " << name << " - Added channel [Voltage Channel]: " << properties.name << std::endl;
		}
		break;
	case Task::CountEdgeChannel:
		error = DAQmxCreateCICountEdgesChan(handle, properties.name.c_str(), properties.assignedChannel.c_str(),
			properties.channelEdge, properties.initialCount, properties.countDirection);
		if (!handleError(error, "Task::addChannel() - CountEdgeChannel"))
		{
			std::cerr << "Task " << name << " - Added channel [CountEdge Channel]: " << properties.name << std::endl;
		}
		break;
	default:
		std::cerr << "Task::addChannel() error: no such channel type " << type << std::endl;
		break;
	}

	this->properties.channel = properties;
}

void Task::addTimer(TaskProperties::Timer properties)
{
	float64 msrate;
	DAQmxGetSampClkMaxRate(handle, &msrate);

	float64 myrate = properties.sampleRate;

	if (myrate > msrate)
	{
		std::cerr << "Task " << name << " with rate @ " << myrate / 1000.0 << " kS/s exceeds max device sample rate of " << msrate / 1000.0 << " kS/s" << std::endl;
		std::cerr << "Setting the rate to the max device rate [" << msrate / 1000.0 << " kS/s]" << std::endl;
		myrate = msrate;
		properties.sampleRate = msrate;
	}

	int32 error = DAQmxCfgSampClkTiming(handle, properties.source.c_str(), myrate, properties.activeEdge, properties.sampleMode, properties.samplesPerChannel);
	if (!handleError(error, "Task::addTimer()"))
	{
		std::cerr << "Task " << name << " - Added timer @ " << myrate / 1000.0 << "kS/s" << std::endl;
	}

	this->properties.timer = properties;
}

void Task::addEventCallback(ENCHandler callback, void * callbackData, uInt32 packetSamples, uInt32 options)
{
	int32 error = DAQmxRegisterEveryNSamplesEvent(handle, DAQmx_Val_Acquired_Into_Buffer, packetSamples, options, callback, callbackData);
	if (!handleError(error, "Task::addEventCallback()"))
	{
		std::cerr << "Task " << name << " - Registered Event Callback: ENC" << std::endl;
	}
}

void Task::addEventCallback(DECHandler callback, void * callbackData)
{
	int32 error = DAQmxRegisterDoneEvent(handle, 0, callback, callbackData);
	if (!handleError(error, "Task::addEventCallback()"))
	{
		std::cerr << "Task " << name << " - Registered Event Callback: DEC" << std::endl;
	}
}
