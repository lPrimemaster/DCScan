#pragma once
#include <iostream>
#include <curl/curl.h>

class ProtocolPacket
{
public:
	static void uploadFTP(const char* filename);
};

