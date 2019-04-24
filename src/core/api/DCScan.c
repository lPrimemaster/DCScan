#include <stdio.h>
#include "include/DCScan.h"
#include "execution.h"

#ifdef _WIN32
#include <Windows.h>
#endif

DCS_API void DCScan_StartAcq(DCScan_Context * acquisition)
{
	int value = executeDefault(acquisition);
}

DCS_API void DCScan_StopAcq(DCScan_Context* acquisition)
{

}

DCS_API void DCScan_StartupSettings(DCScan_Settings* settings)
{
	
}

DCS_API DCScan_Context* DCScan_CreateAcqContext(const char* config)
{
	DCScan_Context* ret_acq = (DCScan_Context*)malloc(sizeof(DCScan_Context));
	if (ret_acq != NULL)
	{
		if (config != NULL)
			strcpy_s(ret_acq->filename, strlen(config) * sizeof(char), config);
		else
			strcpy_s(ret_acq->filename, 18 * sizeof(char), "default.ini");
		return ret_acq;
	}
	return NULL;
}

DCS_API void DCScan_FreeAcqContext(DCScan_Context* acquisition)
{
	if(acquisition != NULL)
		free(acquisition);
}
