#include "ThreadManager.h"
#include <string>
#include <sstream>
#ifdef _WIN32
#include <Windows.h>
#endif

ThreadManager::ThreadManager()
{
	pool_used = new bool[THREAD_CONCURRENCY];
	for (int i = 0; i < THREAD_CONCURRENCY; i++) pool_used[i] = false;

	random_seed();

#ifdef _WIN32
	//HANDLE hConsole_c = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	HANDLE hConsole_c = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleActiveScreenBuffer(hConsole_c);

	//Assuming the console size wont be changed
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns, rows;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

#endif

	//Thread for looking at status without notifications
	auto lf = [=] (std::atomic_int*, void*) -> void
	{
#ifdef _WIN32
		const int y = rows - 1;
		const char *str[2] = { "> IDLE   ", "> RUNNING" };
#endif

		//This thread does not respond to the THREAD_HALT flag
		while (deinit.load() != 1)
		{


#ifdef _WIN32
			for (int i = 0; i < THREAD_CONCURRENCY; i++)
			{
				long ri = pool_used[i] ? 1 : 0;
				COORD pos = { 0, y - i };
				DWORD len = strlen(str[ri]);
				DWORD dwBytesWritten = 0;
				WriteConsoleOutputCharacter(hConsole_c, str[ri], len, pos, &dwBytesWritten);
#endif
			}
			for (int i = 0; i < THREAD_CONCURRENCY; i++)
			{
				if (!pool_used[i])
				{
					continue;
				}
				else
				{
					//Skip first letters of running text indicator (columns)
					char stch[50] = "- Thread #";
					strcat_s(stch, std::to_string(i).c_str());
					strcat_s(stch, " - ID: ");
					auto it = pool_pos.begin();
					for (int z = 0; z < THREAD_CONCURRENCY; z++)
					{
						bool end = it == pool_pos.end();

						if (it->second == i)
						{
							std::stringstream ss;
							ss << "0x" << std::hex << it->first;
							strcat_s(stch, ss.str().c_str());
							break;
						}
						if (!end)
						{
							it++;
						}
						else
						{
							break;
						}
					}

					COORD pos = { 11, y - i };
					DWORD len = strlen(stch);
					DWORD dwBytesWritten = 0;
					WriteConsoleOutputCharacter(hConsole_c, stch, len, pos, &dwBytesWritten);
				}
			}
			//Prevent this gui from showing up too fast
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			DWORD dwBytesWrittenSC = 0;
			FillConsoleOutputCharacter(hConsole_c, (TCHAR) ' ', csbi.dwSize.X * csbi.dwSize.Y,
				COORD{ 0, SHORT(y - THREAD_CONCURRENCY) }, &dwBytesWrittenSC);
			std::this_thread::yield();
		}

#ifdef _WIN32
		CloseHandle(hConsole_c);
#endif
		//Flag ending
		deinit.store(2);
	};

	std::thread::id id = addThread(lf, nullptr);
	//Detach this thread, as it should not be available to the pool
	joinThreadAsync(id);
}

ThreadManager::~ThreadManager()
{
	//Signal manager thread to stop
	deinit.store(1);

	for (auto o : obj)
	{
		if (o.second->joinable())
		{
			flags[o.first]->store(THREAD_HALT);
			o.second->join();
			delete o.second;
			std::cerr << "Thread 0x" << std::hex << o.first << " is joining this thread [0x" << std::hex << std::this_thread::get_id() << "]. Waiting" << std::endl;
		}
		else
		{
			flags[o.first]->store(THREAD_HALT);
			o.second->detach();
			std::cerr << "Thread 0x" << std::hex << o.first << " is not joinable. Halting and detaching" << std::endl;
		}
		delete flags[o.first];
	}

	flags.clear();
	obj.clear();

	//Wait for thread to end - to delete pool_used var
	while (deinit.load() != 2);

	delete[] pool_used;
}

std::thread::id ThreadManager::addThread(std::function<void(std::atomic<int>*, void*)> threadFunction, void* threadData)
{
	std::thread * thread;
	/* Init a thread with the running status obviously */
	std::atomic<int> * flag = new std::atomic<int>(THREAD_RUN);
	if (threadData != nullptr)
		thread = new std::thread(threadFunction, flag, threadData);
	else
		thread = new std::thread(threadFunction, flag, nullptr);

	std::thread::id id = thread->get_id();

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
	if (obj[threadId]->joinable())
	{
		flags[threadId]->store(THREAD_HALT);
		obj[threadId]->join();

		std::cerr << "Thread 0x" << std::hex << threadId << " joined successfully" << std::endl;

		delete flags[threadId];
		delete obj[threadId];

		obj.erase(threadId);
		flags.erase(threadId);

		pool_used[pool_pos[threadId]] = false;
		pool_pos.erase(threadId);

		return true;
	}

	std::cerr << "Thread 0x" << std::hex << threadId << " is not joinable. Ignoring" << std::endl;
	return false;
}

bool ThreadManager::joinThreadAsync(std::thread::id threadId)
{
	flags[threadId]->store(THREAD_HALT);
	obj[threadId]->detach();

	std::cerr << "Thread 0x" << std::hex << threadId << " is being detached" << std::endl;

	delete flags[threadId];
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
		std::cerr << "Thread 0x" << std::hex << threadId << " cannot be halted from this method. Refer to joinThread{Sync,Async} methods" << std::endl;
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
