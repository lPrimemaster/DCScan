#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <map>
#include <NIDAQmx.h>

typedef int32(__cdecl *ENCHandler)(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
typedef int32(__cdecl *DECHandler)(TaskHandle taskHandle, int32 status, void *callbackData);

struct TaskProperties
{
	std::string name;

	struct Channel
	{
		std::string name;
		std::string assignedChannel;
		std::string customScaleName;
		float64 minValue;
		float64 maxValue;
		int32 channelUnits;

		int32 channelEdge;
		int32 initialCount;
		int32 countDirection;
	}channel;

	struct Timer
	{
		std::string source;
		float64 sampleRate;
		int32 activeEdge;
		int32 sampleMode;
		uInt32 samplesPerChannel;
	}timer;
};

class Task
{
public:
	enum ChannelType : int;

	using TMap = std::map<std::string, Task*>;

	Task(const std::string& taskname);
	~Task();

	void start();
	void stop();
	
	void addChannel(ChannelType type, TaskProperties::Channel properties);
	void addTimer(TaskProperties::Timer properties);

	void addEventCallback(ENCHandler callback, void * callbackData, uInt32 packetSamples, uInt32 options);
	void addEventCallback(DECHandler callback, void * callbackData);

	enum ChannelType
	{
		VoltageChannel,
		CountEdgeChannel
	};

	static Task* GetRegisteredTask(std::string name);

	inline bool isRunning() const
	{
		return is_running;
	}

private:
	std::string name;
	TaskHandle handle;

	TaskProperties properties;

	bool is_running = false;

	inline static TMap registeredTasks;
};

