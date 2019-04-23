#include <iostream>
#include <fstream>
#include <string>
#include "core/worker_threads.h"
#include "core/base/Timer.h"
#include "core/base/counter.h"

#include "ctrl/CFlush.h"

#include "core/io/usb_serial.h"
#include "core/io/register.h"

#include "version.h"

//Options for later work : César
//Opt 1 - NI-DAQmx intrinsic handshaking for communication with engines
//Opt 2 - Software timing / event for communication with engines

//TODOS : César
//Motors communication / control implementation
//Api implementation for use in external GUI (for : Tiago ?)
//Fix an error with thread concurrency at ThreadManager.cpp : 67 -> pool erased
//Create a system where there can be more than one .ini file, for multiple configurations
//Fix directory non existent for data or config files
int main(int argc, char* argv[])
{
	//Initialize default windows handle for operation
	CFlush::InitHandle();

	//Redirect cerr to file
	std::ofstream filebuffer("logs/" + GET_VERSION_STR() + ".log");
	std::streambuf *cerrbuf = std::cerr.rdbuf();
	std::cerr.rdbuf(filebuffer.rdbuf());

	//Redirect cout to modified printf - FIXME
	std::stringstream outbuffer;
	std::streambuf *outbuf = std::cout.rdbuf();
	std::cout.rdbuf(outbuffer.rdbuf());

	ThreadManager manager;

	Timestamp ts = Timer::apiTimeSystemHRC();

	std::cout << "Program started... " << std::endl << "Current time - " << ts.year << "/" << ts.month << "/" << ts.day << " " << ts.hour << ":" << ts.min << ":" << ts.sec << ":" << ts.millis << ":" << ts.micros << ":" << ts.nanos << std::endl;
	std::cout << "Main thread [0x" << std::hex << std::this_thread::get_id() << "] started." << std::endl;

	IO::createIniFile("config\\default.ini");
	IO::IniFileData data = IO::readIniFile("config\\default.ini");

	AcquireDataOptions doptions;
	TaskProperties properties;

	properties.name = "task0";
	properties.channel.name = data["AcquireSettings"]["default_channel"].c_str();
	properties.channel.assignedChannel = data["AcquireSettings"]["default_assigned"].c_str();
	properties.channel.channelUnits = atoi(data["AcquireSettings"]["default_units"].c_str());
	properties.channel.customScaleName = "";
	properties.channel.minValue = atof(data["AcquireSettings"]["default_min"].c_str());
	properties.channel.maxValue = atof(data["AcquireSettings"]["default_max"].c_str());

	properties.timer.activeEdge = DAQmx_Val_Rising;
	properties.timer.sampleMode = DAQmx_Val_ContSamps;
	properties.timer.sampleRate = atof(data["AcquireSettings"]["default_timer"].c_str());
	properties.timer.samplesPerChannel = atoi(data["AcquireSettings"]["default_spc"].c_str());
	properties.timer.source = "";

	doptions.tproperties = properties;

	FILE* f;
	fopen_s(&f, std::string(data["IOLocation"]["relative_path"] + "/" 
		+ data["IOLocation"]["name"] + "." 
		+ data["IOLocation"]["extension"]).c_str(), "w");
	if (f == nullptr)
	{
		std::cerr << "IO error: could not find/open the specified file: " 
			<< std::string(data["IOLocation"]["relative_path"] + "/" 
				+ data["IOLocation"]["name"] + "." 
				+ data["IOLocation"]["extension"]) << std::endl;
		return -1;
	}
	fprintf(f, "Packet,Point,Data,Timestamp [Software],Timestamp [Hardware][us],Packet Delta [ms - us]\n");

	//The following nomenclature is to be followed
	//Convert all datatype pointers to intptr_t and then pass them to the required function or thread, unwrapping it latter
	auto tid_0 = manager.addThread(acquireThread, &doptions);
	auto tid_1 = manager.addThread(processThread, convertToIntPointer(f, &doptions));
	//auto tid_2 = manager.addThread(controlThread, NULL);

	CFlush::FlushConsoleStream(&outbuffer);

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	manager.joinThreadSync(tid_0);
	manager.joinThreadSync(tid_1);
	//manager.joinThreadSync(tid_2);

	fclose(f);

	libusb_context ** ctx = NULL;

	usb_serial_init_libusb(ctx, 0);
	usb_serial_device_list lst;
	usb_serial_get_device_list(ctx, &lst);
	usb_serial_print_device_list(&lst);

	//lst works here for access only

	ssize_t s = usb_serial_get_device_index_from_id(&lst, 0x0717B);

	std::cout << std::dec << "Index of found device [by id]: " << s << std::endl;

	s = usb_serial_get_device_index_from_name(&lst, "USB Optical Mouse");

	std::cout << std::dec << "Index of found device [by name]: " << s << std::endl;

	usb_serial_free_device_list(&lst);
	usb_serial_deinit_libusb(ctx);

	std::cout << "Program uptime: " << Timer::apiUptimeString() << std::endl;

	//CFlush::ClearConsole(0, CFlush::rows - THREAD_CONCURRENCY);
	CFlush::FlushConsoleStream(&outbuffer);

	//Always revert the .ini file to default for safety purposes and test only
	IO::createIniFile("config\\default.ini");

	CFlush::FlushConsoleStream(&outbuffer);

	//Close the handle only when all user threads stopped
	CFlush::CloseHandle();

	return 0;
}

int test()
{
	char   ComPortName[] = "COM4";
	BOOL Status;

	HANDLE hComm = CreateFile(ComPortName,                       // Name of the Port to be Opened
		GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
		0,                                 // No Sharing, ports cant be shared
		NULL,                              // No Security
		OPEN_EXISTING,                     // Open existing port only
		0,                                 // Non Overlapped I/O
		NULL);                             // Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE)
		std::cout << "\n   Port " << ComPortName << " Error - Not Opened..." << std::endl;
	else
		std::cout << "\n   Port " << ComPortName << " Opened!" << std::endl;

	DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);     //retreives  the current settings

	if (Status == FALSE)
		printf("\n   Error! in GetCommState()");

	dcbSerialParams.BaudRate = 921600;      // Setting BaudRate = 921600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;      // Setting Parity = None 

	Status = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB 

	if (Status == FALSE)
	{
		printf("\n   Error! in Setting DCB Structure");
	}
	else
	{
		printf("\n   Setting DCB Structure Successfull\n");
		printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
		printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
		printf("\n       StopBits = %d", dcbSerialParams.StopBits);
		printf("\n       Parity   = %d", dcbSerialParams.Parity);
	}

	/*------------------------------------ Setting Timeouts --------------------------------------------------*/

	/*COMMTIMEOUTS timeouts = { 0 };

	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (SetCommTimeouts(hComm, &timeouts) == FALSE)
		printf("\n   Error! in Setting Time Outs");
	else
		printf("\n\n   Setting Serial Port Timeouts Successfull");*/

	char   lpBuffer[] = "1VA15;1PA0;1WS1;1TP?\r";// lpBuffer should be  char or byte array, otherwise write wil fail
	DWORD  dNoOFBytestoWrite;               // No of bytes to write into the port
	DWORD  dNoOfBytesWritten = 0;           // No of bytes written to the port

	dNoOFBytestoWrite = sizeof(lpBuffer); // Calculating the no of bytes to write into the port

	Status = WriteFile(hComm,               // Handle to the Serialport
		lpBuffer,            // Data to be written to the port 
		dNoOFBytestoWrite,   // No of bytes to write into the port
		&dNoOfBytesWritten,  // No of bytes written to the port
		NULL);

	if (Status == TRUE)
		printf("\n\n    %s - Written to %s", lpBuffer, ComPortName);
	else
		printf("\n\n   Error %d in Writing to Serial Port\n", GetLastError());

	DWORD dwEventMask;                     // Event mask to trigger
	char  TempChar;                        // Temperory Character
	char  SerialBuffer[256];               // Buffer Containing Rxed Data
	DWORD NoBytesRead;                     // Bytes read by ReadFile()
	int i = 0;

	Status = SetCommMask(hComm, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

	if (Status == FALSE)
		printf("\n\n    Error! in Setting CommMask");
	else
		printf("\n\n    Setting CommMask successfull");


	/*------------------------------------ Setting WaitComm() Event   ----------------------------------------*/

	printf("\n\n    Waiting for Data Reception");

	Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received

	/*-------------------------- Program will Wait here till a Character is received ------------------------*/

	if (Status == FALSE)
	{
		printf("\n    Error! in Setting WaitCommEvent()");
	}
	else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
	{
		printf("\n\n    Characters Received = ");
		while(true)
		{
			Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
			SerialBuffer[i] = TempChar;
			i++;
			if (TempChar == '\n') break;
		}



		/*------------Printing the RXed String to Console----------------------*/

		int j = 0;
		for (j = 0; j < i - 1; j++)		// j < i-1 to remove the dupliated last character
			printf("%c", SerialBuffer[j]);
		putchar('\n');
	}

	CloseHandle(hComm);//Closing the Serial Port
	return 0;
}