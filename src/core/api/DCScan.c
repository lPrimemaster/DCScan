#include <stdio.h>
#include "include/DCScan.h"
#include "execution.h"

DCS_API void DCScan_StartAcq(DCScan_Acquisition * acquisition)
{
	int value = executeDefault(acquisition);
}

DCS_API void DCScan_StopAcq(DCScan_Acquisition* acquisition)
{

}

DCS_API void DCScan_StartupSettings(DCScan_Settings* settings)
{
	
}
