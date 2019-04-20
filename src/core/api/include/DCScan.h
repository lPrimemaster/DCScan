#ifdef _API_EXPORTING
#define DCS_API  __declspec(dllexport)
#else
#define DCS_API  __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif
	struct _dcs_acquisition
	{
		//placeholder for an interop struct for the acquisition specifications
		int a;
	};

	struct _dcs_stupoptions
	{
		//placeholder for an interop struct for the default settings
		int a;
	};

	typedef struct _dcs_acquisition DCScan_Acquisition;
	typedef struct _dcs_stupoptions DCScan_Settings;

	/* Starts the DSC Acquisition process remotely */
	DCS_API void DCScan_StartAcq(DCScan_Acquisition* acquisition);

	/* Stops the DSC Acquisition process remotely */
	DCS_API void DCScan_StopAcq(DCScan_Acquisition* acquisition);

	/* Tells the C API what settings to store in the .ini file (or registry) for the next startup */
	DCS_API void DCScan_StartupSettings(DCScan_Settings* settings);
#ifdef __cplusplus
}
#endif