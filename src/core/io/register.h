#pragma once
#include <vector>
#include <map>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif

namespace IO
{
	typedef std::map<const char*, std::map<std::string, std::string>> IniFileData;

	struct IniFileProperties
	{
		const std::vector<const char*> section = { "Version", "IOLocation", "UpdateServer", "AcquireSettings", "ControlSettings" };
		const std::map<const char*, std::vector<const char*>> sub_sec =
		{
			{ "Version",			{ "major", "minor" } },											//Version
			{ "IOLocation",			{ "name", "relative_path", "absolute_path", "extension" } },	//IOLocation
			{ "UpdateServer",		{ "protocol", "url" } },										//UpdateServer
			{ "AcquireSettings",	{	"default_timer",
										"default_channel",
										"default_assigned",
										"default_units",
										"default_min",
										"default_max",
										"default_spc"
									} 
			},                                                             //AcquireSettings

			{ "ControlSettings",	{	"default_delta",
										"motor_types",
										"measure_units",
										"measure_precision",
										"feedback_config",
										"ampio_config",
										"e_stop_config",
										 "error_rep_config",
										"hardware_limit",
										"software_limit",
										"encoder_resolution",
										"encoder_fs_resolution",
										"micro_step_factor",
										"avg_motor_voltage",
										"max_motor_currrent",
										"gear_constant",
										"tachometer_gain",
										"left_travel_limit",
										"right_travel_limit",
										"trajectory_mode",
										"home_search_mode",
										"max_velocity",
										"sel_velocity",
										"jog_highspeed",
										"jog_lowspeed",
										"home_highspeed",
										"step_base_velocity",
										"max_acceleration",
										"sel_acceleration",
										"sel_deceleration",
										"e_stop_deceleration",
										"jerk_rate",
										"pid_p_gain",
										"pid_i_gain",
										"pid_d_gain",
										"veclocity_ff_gain",
										"acceleration_ff_gain",
										"sat_integral_fac",
										"max_following_error",
										"position_deadband",
										"closed_loop_interval",
										"motor_torque_ratio",
										"motor_torque_interval",
										"master_slave_rel",
										"master_slave_ratio",
										"ms_jog_vel_interval",
										"ms_jog_coef_A",
										"ms_jog_coef_B",
										"backlash_compensation",
										"linear_compensation"
									} 
			}											//ControlSettings
		};
	};

	void createIniFile(LPCTSTR name);
	IniFileData readIniFile(LPCTSTR name);
	IniFileData readCfgFile(LPCTSTR name);

	bool valueHasBracket(const std::string& str);

	template<typename T>
	inline std::vector<T> convertBracketValue(const std::string& str);


	template<> //Does not skip with no bracket
	inline std::vector<float> convertBracketValue(const std::string& str)
	{
		std::vector<float> out;
		std::string::const_iterator it = str.begin();
		if (*it != '{')
		{
			//error
		}
		else
		{
			char buffer[32] = { 0 };
			int i = 0;
			while (++it != str.end())
			{
				if (*it == ' ')
				{
					continue;
				}
				else if(*it != ',' && *it != '}')
				{
					buffer[i++] = *it;
				}
				else
				{
					out.push_back(atof(buffer));
					memset(buffer, 0, 32);
					i = 0;
				}
			}
			return out;
		}
	}

	template<> //Does not skip with no bracket
	inline std::vector<int> convertBracketValue(const std::string& str)
	{
		std::vector<int> out;
		std::string::const_iterator it = str.begin();
		bool hex = false;
		if (*it != '{')
		{
			//error
		}
		else
		{
			char buffer[32] = { 0 };
			int i = 0;
			while (++it != str.end())
			{
				if (*it == ' ')
				{
					continue;
				}
				else if (*it != ',' && *it != '}')
				{
					if (*it == 'H')
					{
						hex = true;
					}
					else
					{
						buffer[i++] = *it;
					}
				}
				else
				{
					out.push_back(strtol(buffer, NULL, hex ? 16 : 10));
					memset(buffer, 0, 32);
					hex = false;
					i = 0;
				}
			}
			return out;
		}
	}

	template<> //Skips with no bracket
	inline std::vector<std::string> convertBracketValue(const std::string& str)
	{
		std::vector<std::string> out;
		std::string::const_iterator it = str.begin();
		if (*it != '{')
		{
			//No bracket, keep value
			out.push_back(str);
			return out;
		}
		else
		{
			char buffer[32] = { 0 };
			int i = 0;
			while (++it != str.end())
			{
				if (*it == ' ')
				{
					continue;
				}
				else if (*it != ',' && *it != '}')
				{
					buffer[i++] = *it;
				}
				else
				{
					out.push_back(buffer);
					memset(buffer, 0, 32);
					i = 0;
				}
			}
			return out;
		}
	}
}
