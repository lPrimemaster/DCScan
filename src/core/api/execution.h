#pragma once

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct _dcs_acquisition DCScan_Context;

	int executeDefault(DCScan_Context* acquisition);

#ifdef __cplusplus
}
#endif