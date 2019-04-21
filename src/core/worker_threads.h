#pragma once
#include "../ctrl/ThreadManager.h"
#include "Task.h"

struct AcquireDataOptions
{
	TaskProperties tproperties;
};

//This thread acquires the data via DAQmx functionalities
void acquireThread(std::atomic<int>* flags, void* data);

//This thread processes the acquired data via an ordered list (deque) 
void processThread(std::atomic<int>* flags, void* data);

//This thread controls the engines using ASCII commands sent in binary to the controller
void controlThread(std::atomic<int>* flags, void* data);
