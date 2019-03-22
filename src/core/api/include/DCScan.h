#ifdef _API_EXPORTING
#define DCS_API  __declspec(dllexport)
#else
#define DCS_API  __declspec(dllimport)
#endif

struct _dcs_acquisition
{
	//placeholder for an interop struct
	int a;
};

typedef struct _dcs_acquisition DCScan_Acquisition;

DCS_API void DCScan_StartAcq(DCScan_Acquisition* acquisition);