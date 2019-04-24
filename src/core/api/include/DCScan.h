#ifdef _API_EXPORTING
#define DCS_API  __declspec(dllexport)
#else
#define DCS_API  __declspec(dllimport)
#endif

//All extern definitions must be visible here

#define DCS_FIXED_DURATION 0x00
#define DCS_UNDEF_DURATION 0x01

#ifdef __cplusplus
extern "C" {
#endif

	struct _dcs_acquisition
	{
		char filename[64];
	};

	struct _dcs_stupoptions
	{
		//placeholder for an interop struct for the default settings
		int a;
	};

	typedef struct _dcs_acquisition DCScan_Context;
	typedef struct _dcs_stupoptions DCScan_Settings;

	/* Starts the DSC Acquisition process remotely */
	DCS_API void DCScan_StartAcq(DCScan_Context* acquisition);

	/* Stops the DSC Acquisition process remotely */
	DCS_API void DCScan_StopAcq(DCScan_Context* acquisition);

	/* Tells the C API what settings to store in the .ini file (or registry) for the next startup */
	DCS_API void DCScan_StartupSettings(DCScan_Settings* settings);

	/* Creates a new context to work with, if flags is NULL then all the default values are loaded from the default .ini file */
	DCS_API DCScan_Context* DCScan_CreateAcqContext(const char* config);

	/* Deletes a context */
	DCS_API void DCScan_FreeAcqContext(DCScan_Context* acquisition);
#ifdef __cplusplus
}
#endif