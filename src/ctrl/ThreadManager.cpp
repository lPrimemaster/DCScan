#include "ThreadManager.h"
#include "../ctrl/CFlush.h"
#include <string>
#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#endif

bool* ThreadManager::pool_used = nullptr;
std::atomic<int> ThreadManager::deinit = 0;
std::unordered_map<std::thread::id, unsigned> ThreadManager::pool_pos;

//Thread for looking at status without notifications
void threadHelpTip(std::atomic_int* at, void* data)
{
	const int y = CFlush::rows - 1;

	ThreadManager* manager = (ThreadManager*)(data);

	//This thread does not respond to the THREAD_HALT flag
	while (ThreadManager::deinit.load() != 1)
	{
		for (int i = 0; i < THREAD_CONCURRENCY; i++)
		{
			if (!ThreadManager::pool_used[i])
			{
				CFlush::printXYColor(CFlush::TEXT_GRAY, { 0,  (SHORT)(y - i)},  "Thread #%d", i);
				CFlush::printXYColor(CFlush::TEXT_RED,  { 12, (SHORT)(y - i) }, " < IDLE    ");
				CFlush::printXYColor(CFlush::TEXT_RED,  { 26, (SHORT)(y - i) }, "                               ");
			}
			else
			{
				CFlush::printXYColor(CFlush::TEXT_GRAY,  { 0,  (SHORT)(y - i) }, "Thread #%d", i);
				CFlush::printXYColor(CFlush::TEXT_GREEN, { 12, (SHORT)(y - i) }, " < RUNNING ");
				
				auto it = ThreadManager::pool_pos.begin();

				while (!(!ThreadManager::pool_pos.empty() && it->second == i) && !(it == ThreadManager::pool_pos.end())) 
				{
					it++;
				}
				if (it != ThreadManager::pool_pos.end())
				{
					if (manager->flags.at(it->first)->load() == THREAD_ENDED)
					{
						//This should be a callback - but impact is minimal...
						manager->joinThreadSync(it->first);
					}

					std::stringstream ss;
					ss << it->first;
					//CFlush::printXYColor(CFlush::TEXT_GRAY, { 25, (SHORT)(y - i) }, " [%s] ", CFlush::formatString("%#06X", id));
					CFlush::printXYColor(CFlush::TEXT_GRAY, { 25, (SHORT)(y - i) }, " [ID: %s]                ", ss.str().c_str());
				}
			}
		}

		//Prevent this gui from showing up too fast
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	for (auto o : manager->obj)
	{
		if (o.second->joinable())
		{
			manager->flags[o.first]->store(THREAD_HALT);
			o.second->join();
			delete o.second;
			std::cerr << "Thread 0x" << std::hex << std::uppercase << o.first << std::nouppercase << " is joining this thread [0x" << std::hex << std::uppercase << std::this_thread::get_id() << std::nouppercase << std::dec << "]. Waiting..." << std::endl;
		}
		else
		{
			manager->flags[o.first]->store(THREAD_HALT);
			o.second->detach();
			std::cerr << "Thread 0x" << std::hex << std::uppercase << o.first << std::nouppercase << " is not joinable. Halting and detaching." << std::dec << std::endl;
		}
		delete manager->flags[o.first];
	}

	manager->flags.clear();
	manager->obj.clear();

	delete[] manager->pool_used;

	//Flag ending
	ThreadManager::deinit.store(2);
};

ThreadManager::ThreadManager()
{
	ThreadManager::pool_used = new bool[THREAD_CONCURRENCY];
	for (int i = 0; i < THREAD_CONCURRENCY; i++) ThreadManager::pool_used[i] = false;

	std::cerr << "MAX_THREAD_CONCURRENCY = " << THREAD_CONCURRENCY << std::endl;

	std::thread::id id = addThread(threadHelpTip, this);
	//Detach this thread, as it should not be available to the pool
	joinThreadAsync(id);
}

ThreadManager::~ThreadManager()
{
	//Signal manager thread to stop
	deinit.store(1);

	//Wait for thread to end - to proceed
	while (deinit.load() != 2);
}

std::thread::id ThreadManager::addThread(Tfunc threadFunction, void* threadData)
{
	std::thread * thread;
	/* Init a thread with the running status */
	std::atomic<int> * flag = new std::atomic<int>(THREAD_RUN);
	if (threadData != nullptr)
		thread = new std::thread(threadFunction, flag, threadData);
	else
		thread = new std::thread(threadFunction, flag, nullptr);

	std::thread::id id = thread->get_id();

	std::cerr << "Thread 0x" << std::hex << std::uppercase << id << std::nouppercase << " started." << std::dec << std::endl;

	obj.emplace(id, thread);
	flags.emplace(id, flag);

	//Check first available slot for the flag
	int i;
	for (i = 0; i < THREAD_CONCURRENCY; i++) if (!pool_used[i]) break;

	pool_used[i] = true;
	pool_pos.emplace(id, i);

	return id;
}

bool ThreadManager::joinThreadSync(std::thread::id threadId)
{
	if (obj.find(threadId) == obj.end())
	{
		std::cerr << "Thread 0x" << std::hex << std::uppercase << threadId << std::nouppercase << " already joined. Ignoring." << std::dec << std::endl;
		return false;
	}

	if (obj[threadId]->joinable())
	{
		flags[threadId]->store(THREAD_HALT);
		obj[threadId]->join();

		std::cerr << "Thread 0x" << std::hex << std::uppercase << threadId << std::nouppercase << " joined successfully." << std::dec << std::endl;

		delete flags[threadId];
		delete obj[threadId];

		obj.erase(threadId);
		flags.erase(threadId);

		pool_used[pool_pos[threadId]] = false;
		pool_pos.erase(threadId);

		return true;
	}

	std::cerr << "Thread 0x" << std::hex << std::uppercase << threadId << std::nouppercase << " is not joinable. Ignoring." << std::dec << std::endl;
	return false;
}

bool ThreadManager::joinThreadAsync(std::thread::id threadId)
{
	flags[threadId]->store(THREAD_HALT);
	obj[threadId]->detach();

	std::cerr << "Thread 0x" << std::hex << std::uppercase << threadId << std::nouppercase << " is being detached." << std::dec << std::endl;

	delete flags[threadId];
	if (obj[threadId] != nullptr)
		delete obj[threadId];

	obj.erase(threadId);
	flags.erase(threadId);

	pool_used[pool_pos[threadId]] = false;
	pool_pos.erase(threadId);

	return true;
}

bool ThreadManager::isIdle(std::thread::id threadId)
{
	return false;
}

bool ThreadManager::signalThreadSetStatus(std::thread::id threadId, int status)
{
	if (status == THREAD_HALT)
	{
		std::cerr << "Thread 0x" << std::hex << std::uppercase << threadId << std::nouppercase << " cannot be halted from this method. Refer to joinThread{Sync,Async} methods." << std::dec << std::endl;
		return false;
	}
	flags[threadId]->store(status);
	return true;
}

int ThreadManager::signalThreadGetStatus(std::thread::id threadId)
{
	int status = flags[threadId]->load();
	return status;
}
