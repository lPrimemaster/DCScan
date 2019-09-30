#include "PerfCount.h"
#include "ThreadManager.h"
#include "CFlush.h"

PDH_HQUERY PerfCount::query;
HANDLE PerfCount::Event;
bool PerfCount::fIsWorking;
int PerfCount::time;

std::vector<CounterInfo> PerfCount::vciSelectedCounters;

static DWORD GetProcessorCount()
{
	SYSTEM_INFO sysinfo;
	DWORD dwNumberOfProcessors;
	GetSystemInfo(&sysinfo);
	dwNumberOfProcessors = sysinfo.dwNumberOfProcessors;

	return dwNumberOfProcessors;
}

static std::vector<PCTSTR> GetProcessNames()
{
	DWORD dwProcessID[SIZE];
	DWORD cbProcess;
	DWORD cProcessID;
	BOOL fResult = FALSE;
	DWORD index;


	HANDLE hProcess;
	HMODULE lphModule[SIZE];
	DWORD cbNeeded;
	int len;


	std::vector<PCTSTR> vProcessNames;


	TCHAR* szProcessName;
	TCHAR* szProcessNameWithPrefix;


	fResult = EnumProcesses(dwProcessID, sizeof(dwProcessID), &cbProcess);


	if (!fResult)
	{
		szProcessName = NULL;
		szProcessNameWithPrefix = NULL;
		return vProcessNames;
	}


	cProcessID = cbProcess / sizeof(DWORD);


	for (index = 0; index < cProcessID; index++)
	{
		szProcessName = new TCHAR[MAX_PATH];
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
			PROCESS_VM_READ,
			FALSE, dwProcessID[index]);
		if (NULL != hProcess)
		{
			if (EnumProcessModulesEx(hProcess, lphModule, sizeof(lphModule),
				&cbNeeded, LIST_MODULES_ALL))
			{
				if (GetModuleBaseName(hProcess, lphModule[0], szProcessName,
					MAX_PATH))
				{
					len = _tcslen(szProcessName);
					_tcscpy(szProcessName + len - 4, TEXT("\0"));

					bool fProcessExists = false;
					int count = 0;
					szProcessNameWithPrefix = new TCHAR[MAX_PATH];
					_stprintf(szProcessNameWithPrefix, TEXT("%s"), szProcessName);
					do
					{
						if (count > 0)
						{
							_stprintf(szProcessNameWithPrefix, TEXT("%s#%d"), szProcessName, count);
						}
						fProcessExists = false;
						for (auto it = vProcessNames.begin(); it < vProcessNames.end(); it++)
						{
							if (_tcscmp(*it, szProcessNameWithPrefix) == 0)
							{
								fProcessExists = true;
								break;
							}
						}
						count++;
					} while (fProcessExists);

					vProcessNames.push_back(szProcessNameWithPrefix);
				}
			}
		}
	}
	szProcessName = NULL;
	szProcessNameWithPrefix = NULL;
	return vProcessNames;
}

static std::vector<PCTSTR> GetValidCounterNames()
{
	std::vector<PCTSTR> validCounterNames;
	DWORD dwNumberOfProcessors = GetProcessorCount();
	DWORD index;
	std::vector<PCTSTR> vszProcessNames;
	TCHAR* szCounterName;


	validCounterNames.push_back(TEXT("\\Processor(_Total)\\% Processor Time"));
	validCounterNames.push_back(TEXT("\\Processor(_Total)\\% Idle Time"));


	for (index = 0; index < dwNumberOfProcessors; index++)
	{
		szCounterName = new TCHAR[MAX_PATH];
		_stprintf(szCounterName, TEXT("\\Processor(%u)\\%% Processor Time"), index);
		validCounterNames.push_back(szCounterName);
		szCounterName = new TCHAR[MAX_PATH];
		_stprintf(szCounterName, TEXT("\\Processor(%u)\\%% Idle Time"), index);
		validCounterNames.push_back(szCounterName);
	}


	vszProcessNames = GetProcessNames();


	for (auto element = vszProcessNames.begin();
		element < vszProcessNames.end();
		element++)
	{
		szCounterName = new TCHAR[MAX_PATH];
		_stprintf(szCounterName, TEXT("\\Process(%s)\\%% Processor Time"), *element);
		validCounterNames.push_back(szCounterName);
	}

	szCounterName = NULL;
	return validCounterNames;
}

void PerfCount::Init()
{
	fIsWorking = false;

	time = 0;

	PDH_STATUS status;

	status = PdhOpenQuery(NULL, 0, &query);

	if (status != ERROR_SUCCESS)
	{
		return;
	}

	Event = CreateEvent(NULL, FALSE, FALSE, L"MyEvent");

	if (Event == NULL)
	{
		return;
	}

	fIsWorking = true;
}

void PerfCount::Record(std::atomic<int>* flags, void* data)
{
	while (flags->load() == THREAD_RUN)
	{
		PDH_STATUS status;
		ULONG CounterType;
		ULONG WaitResult;
		PDH_FMT_COUNTERVALUE DisplayValue;

		status = PdhCollectQueryData(query);

		if (status != ERROR_SUCCESS)
		{
			return;
		}

		status = PdhCollectQueryDataEx(query, SAMPLE_INTERVAL, Event);

		if (status != ERROR_SUCCESS)
		{
			return;
		}

		WaitResult = WaitForSingleObject(Event, INFINITE);

		if (WaitResult == WAIT_OBJECT_0)
		{
			for (auto it = vciSelectedCounters.begin(); it < vciSelectedCounters.end(); it++)
			{
				status = PdhGetFormattedCounterValue(it->counter, PDH_FMT_DOUBLE, &CounterType, &DisplayValue);

				if (status != ERROR_SUCCESS)
				{
					continue;
				}

				Log log;
				log.time = time;
				wcscpy(log.cname, it->counterName);
				log.value = DisplayValue.doubleValue;
				it->logs.push_back(log);

				CFlush::printXYColor(CFlush::Color::TEXT_GREEN, { 0, CFlush::rows - (SHORT)20 }, "CPU Usage %s %%", CFlush::formatString("%lf", log.value).c_str());
			}
		}

		time++;
	}
}

double PerfCount::GetLatestCpuUsage()
{
	//return vciSelectedCounters.at(0).latest.load();
	return 0.0;
	//TODO : Get a single processor status -> Skip all this for now
	/*for (auto it = vciSelectedCounters.begin(); it < vciSelectedCounters.end(); it++)
	{
		if (wcscmp(it->counterName, L""))
		{
			double latest = it->latest.load();
			return latest;
		}
	}
	return 0.0;*/
}

void PerfCount::PrintValidProcTimes()
{
	auto vec = GetValidCounterNames();
	for (auto l : vec)
	{
		std::cout << l << std::endl;
	}
}

void PerfCount::AddCounter(PCWSTR name)
{
	if (fIsWorking)
	{
		PDH_STATUS status;
		CounterInfo ci;
		ci.counterName = name;
		status = PdhAddCounter(query, ci.counterName, 0, &ci.counter);

		if (status != ERROR_SUCCESS)
		{
			return;
		}

		vciSelectedCounters.push_back(ci);
	}
}
