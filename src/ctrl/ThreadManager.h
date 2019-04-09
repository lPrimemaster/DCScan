#pragma once
#include <iostream>
#include <thread>
#include <unordered_map>
#include <functional>
#include <atomic>

#include "../core/base/common.h"

#define THREAD_HALT 0
#define THREAD_RUN 1
#define THREAD_SUSPEND 2
#define THREAD_WAIT 3

const unsigned int THREAD_CONCURRENCY = std::thread::hardware_concurrency();

//Threads should only be created from the main (or a single) thread! This class is not thread-safe!
class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	std::thread::id addThread(std::function<void(std::atomic<int>*, void*) > threadFunction, void* threadData);
	bool joinThreadSync(std::thread::id threadId);
	bool joinThreadAsync(std::thread::id threadId);

	bool isIdle(std::thread::id threadId);

	bool signalThreadSetStatus(std::thread::id threadId, int status);
	int  signalThreadGetStatus(std::thread::id threadId);

private:
	std::unordered_map<std::thread::id, std::thread*> obj;
	std::unordered_map<std::thread::id, std::atomic<int>*> flags;
	std::unordered_map<std::thread::id, unsigned> pool_pos;

	bool * pool_used;

	std::atomic<int> deinit = 0;
};

