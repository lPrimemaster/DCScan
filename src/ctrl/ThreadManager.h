#pragma once
#include <iostream>
#include <thread>
#include <unordered_map>
#include <atomic>

#include "../core/base/common.h"

#define THREAD_HALT 0
#define THREAD_RUN 1
#define THREAD_SUSPEND 2
#define THREAD_WAIT 3
#define THREAD_ENDED 4

const unsigned int THREAD_CONCURRENCY = std::thread::hardware_concurrency();

//Function raw pointer preset to avoid <functional>
typedef void (*Tfunc)(std::atomic<int>*, void*);

//Threads should only be created from the main (or a single) thread! This class is not thread-safe!
class ThreadManager
{
public:
	friend void threadHelpTip(std::atomic_int* at, void* data);
	ThreadManager();
	~ThreadManager();

	std::thread::id addThread(Tfunc threadFunction, void* threadData);
	bool joinThreadSync(std::thread::id threadId);
	bool joinThreadAsync(std::thread::id threadId);

	bool isIdle(std::thread::id threadId);

	bool signalThreadSetStatus(std::thread::id threadId, int status);
	int  signalThreadGetStatus(std::thread::id threadId);

private:
	std::unordered_map<std::thread::id, std::thread*> obj;
	std::unordered_map<std::thread::id, std::atomic<int>*> flags;

	static std::unordered_map<std::thread::id, unsigned> pool_pos;
	static bool * pool_used;
	static std::atomic<int> deinit;
};

