#include "version.h"

std::string GET_VERSION_MAJOR()
{
	const IO::IniFileData VERSION_CFG = IO::readCfgFile("config\\version.cfg");
	return VERSION_CFG.at("Version").at("Major");
}

std::string GET_VERSION_MINOR()
{
	const IO::IniFileData VERSION_CFG = IO::readCfgFile("config\\version.cfg");
	return VERSION_CFG.at("Version").at("Minor");
}

std::string GET_VERSION_BUILD()
{
	const IO::IniFileData VERSION_CFG = IO::readCfgFile("config\\version.cfg");
	return VERSION_CFG.at("Version").at("Build");
}

std::string GET_VERSION_TYPE()
{
	const IO::IniFileData VERSION_CFG = IO::readCfgFile("config\\version.cfg");
	return VERSION_CFG.at("Version").at("Type");
}

std::string GET_VERSION_STR()
{
	const IO::IniFileData VERSION_CFG = IO::readCfgFile("config\\version.cfg");
	return std::string("DCScan-" + GET_VERSION_MAJOR() + "." + GET_VERSION_MINOR() + "_" + GET_VERSION_TYPE() + "_Build#" + GET_VERSION_BUILD());
}
