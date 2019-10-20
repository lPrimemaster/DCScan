#include "register.h"
#include "../../version.h"
namespace IO
{
	void createIniFile(LPCTSTR name)
	{
		char path[128];
		char fpath[128];
		GetFullPathName(name, 128, path, NULL);

		WritePrivateProfileString("Version", "major", GET_VERSION_MAJOR().c_str(), path);
		WritePrivateProfileString("Version", "minor", GET_VERSION_MINOR().c_str(), path);

		WritePrivateProfileString("IOLocation", "name", "data_out", path);
		WritePrivateProfileString("IOLocation", "relative_path", "data", path);
		GetFullPathName("data", 128, fpath, NULL);
		WritePrivateProfileString("IOLocation", "absolute_path", fpath, path);
		WritePrivateProfileString("IOLocation", "extension", "csv", path);

		WritePrivateProfileString("UpdateServer", "protocol", "http", path);
		WritePrivateProfileString("UpdateServer", "url", "http://example.com", path);

		WritePrivateProfileString("AcquireSettings", "default_timer", "10000.0", path);
		WritePrivateProfileString("AcquireSettings", "default_channel", "PXI_Slot2/ai0", path);
		WritePrivateProfileString("AcquireSettings", "default_assigned", "", path);
		WritePrivateProfileString("AcquireSettings", "default_units", "10348", path);
		WritePrivateProfileString("AcquireSettings", "default_min", "-10.0", path);
		WritePrivateProfileString("AcquireSettings", "default_max", "10.0", path);
		WritePrivateProfileString("AcquireSettings", "default_spc", "1000", path);

		/* Just a quick doc on this : {a, b, c} -> one type for each engine
			if a == b == c == val use only val */
		/* These values are just references and need to be edited to be usefull */
		/* Some of these values need to be replaced by bracket triplets for all motors */
		/* Default values configured acording with 'esp301_config.txt' configurations */

		WritePrivateProfileString("ControlSettings", "motor_types", "3", path);
		WritePrivateProfileString("ControlSettings", "measure_units", "7", path);
		WritePrivateProfileString("ControlSettings", "measure_precision", "6", path);
		WritePrivateProfileString("ControlSettings", "feedback_config", "200H", path);
		WritePrivateProfileString("ControlSettings", "ampio_config", "923H", path);
		WritePrivateProfileString("ControlSettings", "e_stop_config", "7H", path);
		WritePrivateProfileString("ControlSettings", "error_rep_config", "0H", path);
		WritePrivateProfileString("ControlSettings", "hardware_limit", "24H", path);
		WritePrivateProfileString("ControlSettings", "software_limit", "4H", path);
		WritePrivateProfileString("ControlSettings", "encoder_resolution", "{0.001, 0.001, 0.001}", path);
		WritePrivateProfileString("ControlSettings", "encoder_fs_resolution", "{0.01, 0.01, 0.01}", path);
		WritePrivateProfileString("ControlSettings", "micro_step_factor", "250", path);
		WritePrivateProfileString("ControlSettings", "avg_motor_voltage", "{30.0, 30.0, 60.0}", path);
		WritePrivateProfileString("ControlSettings", "max_motor_currrent", "{1.75, 0.6, 1.5}", path);
		WritePrivateProfileString("ControlSettings", "gear_constant", "0.5", path);
		WritePrivateProfileString("ControlSettings", "tachometer_gain", "0.0", path);
		WritePrivateProfileString("ControlSettings", "left_travel_limit", "0.0", path);
		WritePrivateProfileString("ControlSettings", "right_travel_limit", "0.0", path);
		WritePrivateProfileString("ControlSettings", "trajectory_mode", "1", path);
		WritePrivateProfileString("ControlSettings", "home_search_mode", "1", path);
		WritePrivateProfileString("ControlSettings", "max_velocity", "20.0", path);
		WritePrivateProfileString("ControlSettings", "sel_velocity", "20.0", path);
		WritePrivateProfileString("ControlSettings", "jog_highspeed", "20.0", path);
		WritePrivateProfileString("ControlSettings", "jog_lowspeed", "2.5", path);
		WritePrivateProfileString("ControlSettings", "home_highspeed", "10.0", path);
		WritePrivateProfileString("ControlSettings", "step_base_velocity", "4.0", path);
		WritePrivateProfileString("ControlSettings", "max_acceleration", "80.0", path);
		WritePrivateProfileString("ControlSettings", "sel_acceleration", "40.0", path);
		WritePrivateProfileString("ControlSettings", "sel_deceleration", "40.0", path);
		WritePrivateProfileString("ControlSettings", "e_stop_deceleration", "1000000", path);
		WritePrivateProfileString("ControlSettings", "jerk_rate", "160.0", path);
		WritePrivateProfileString("ControlSettings", "pid_p_gain", "0.0", path);
		WritePrivateProfileString("ControlSettings", "pid_i_gain", "0.0", path);
		WritePrivateProfileString("ControlSettings", "pid_d_gain", "0.0", path);
		WritePrivateProfileString("ControlSettings", "veclocity_ff_gain", "0.0", path);
		WritePrivateProfileString("ControlSettings", "acceleration_ff_gain", "0.0", path);
		WritePrivateProfileString("ControlSettings", "sat_integral_fac", "50000.0", path);
		WritePrivateProfileString("ControlSettings", "max_following_error", "1.0", path);
		WritePrivateProfileString("ControlSettings", "position_deadband", "0", path);
		WritePrivateProfileString("ControlSettings", "closed_loop_interval", "60000", path);
		WritePrivateProfileString("ControlSettings", "motor_torque_ratio", "50.0", path);
		WritePrivateProfileString("ControlSettings", "motor_torque_interval", "1000", path);
		WritePrivateProfileString("ControlSettings", "master_slave_rel", "1", path);
		WritePrivateProfileString("ControlSettings", "master_slave_ratio", "1.0", path);
		WritePrivateProfileString("ControlSettings", "ms_jog_vel_interval", "10", path);
		WritePrivateProfileString("ControlSettings", "ms_jog_coef_A", "0.5", path);
		WritePrivateProfileString("ControlSettings", "ms_jog_coef_B", "0.0", path);
		WritePrivateProfileString("ControlSettings", "backlash_compensation", "0.0003", path);
		WritePrivateProfileString("ControlSettings", "linear_compensation", "0.0", path);
	}

	IniFileData readIniFile(LPCTSTR name)
	{
		char path[128];
		GetFullPathName(name, 128, path, NULL);
		IniFileProperties properties;

		//128 bytes max string on ini file
		IniFileData data;

		for (auto section : properties.section)
		{
			for (auto sub_sec : properties.sub_sec.at(section))
			{
				char buffer[128];
				DWORD error = GetPrivateProfileString(section, sub_sec, "Unknown", buffer, 128, path);
				data[section][sub_sec] = buffer;
			}
		}

		//Version specification

		//IO Path location + name + extension

		//Update server protocol + url

		//Acquisition settings

		return data;
	}

	IniFileData readCfgFile(LPCTSTR name)
	{
		char path[128];
		GetFullPathName(name, 128, path, NULL);
		IniFileProperties properties;

		//64 bytes max string on cfg file
		IniFileData data;

		char buffer[128];
		DWORD error;

		error = GetPrivateProfileString("Version", "Major", "Unknown", buffer, 128, path);
		data["Version"]["Major"] = buffer;
		error = GetPrivateProfileString("Version", "Minor", "Unknown", buffer, 128, path);
		data["Version"]["Minor"] = buffer;

		error = GetPrivateProfileString("Version", "Type", "Unknown", buffer, 128, path);
		data["Version"]["Type"] = buffer;

		error = GetPrivateProfileString("Version", "Build", "Unknown", buffer, 128, path);
		data["Version"]["Build"] = buffer;

		return data;
	}

	bool valueHasBracket(const std::string & str)
	{
		//Only valid bracket is first char before any valid char / however this is not checked here
		if(str.find_first_of('{') != std::string::npos)
			return false;
		return true;
	}
}