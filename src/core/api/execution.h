#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct _dcs_acquisition DCScan_Acquisition;

	int executeDefault(DCScan_Acquisition* acquisition);

#ifdef __cplusplus
}
#endif