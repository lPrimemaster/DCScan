#include "usb_serial.h"
#include <stdio.h>

#define SERIAL_DEFAULT_BAUD 921600
#define SERIAL_DEFAULT_BYTE 8
#define SERIAL_DEFAULT_SBIT ONESTOPBIT
#define SERIAL_DEFAULT_PARY NOPARITY
#define SERIAL_DEFAULT_EOFC '\r'

#define SERIAL_DEFAULT_READ_INTERVAL_TIMEOUT 50
#define SERIAL_DEFAULT_READ_TOTAL_TIMEOUT_CT 50
#define SERIAL_DEFAULT_READ_TOTAL_MULTIPLIER 10
#define SERIAL_DEFAULT_WRITE_TOTAL_TIMEOUT_CT 50
#define SERIAL_DEFAULT_WRITE_TOTAL_MULTIPLIER 10

#define SERIAL_DEFAULT_READ_BUFFER_SIZE 256
#define SERIAL_DEFAULT_READ_RX_EOF '\n'

HANDLE serial_initHandle(LPCSTR portName, DWORD rwAccess, SerialArgs args)
{
	//HANDLE of the COMport device [serial init]
	HANDLE hComm = CreateFile(portName, rwAccess, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hComm == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Serial: Port %s error. Not opened.\n", portName);
		return INVALID_HANDLE_VALUE;
	}
	else
	{
		fprintf(stderr, "Serial: Port %s opened.", portName);
	}

	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	BOOL status = GetCommState(hComm, &dcbSerialParams); //Get the serial parameters

	if (status == FALSE)
		fprintf(stderr, "Serial: GetCommState() error. Using default or suplied values instead.\n");

	/* Defaults - BDR: 921.6kBd - BSZ: 8bits - SBT: OneStop - PAR: NoParity - EOFC: Carriage return*/
	dcbSerialParams.BaudRate = args.baudRate ? args.baudRate : (dcbSerialParams.BaudRate ? dcbSerialParams.BaudRate : SERIAL_DEFAULT_BAUD);
	dcbSerialParams.ByteSize = args.byteSize ? args.byteSize : (dcbSerialParams.ByteSize ? dcbSerialParams.ByteSize : SERIAL_DEFAULT_BYTE);
	dcbSerialParams.StopBits = args.stopBits ? args.stopBits : (dcbSerialParams.StopBits ? dcbSerialParams.StopBits : SERIAL_DEFAULT_SBIT);
	dcbSerialParams.Parity   = args.parity   ? args.parity   : (dcbSerialParams.Parity   ? dcbSerialParams.Parity   : SERIAL_DEFAULT_PARY);
	dcbSerialParams.EofChar  = args.eofChar  ? args.eofChar  : (dcbSerialParams.EofChar  ? dcbSerialParams.EofChar  : SERIAL_DEFAULT_EOFC);

	status = SetCommState(hComm, &dcbSerialParams); //[Re]configure the port with DCB params

	if (status == FALSE)
	{
		fprintf(stderr, "Serial: SetCommState() error. Using default values, if applicable.\n");
		fprintf(stderr, "Serial: Set COM DCB Structure Fail. Using intrinsic values, if applicable.\n");
		fprintf(stderr, "		Baudrate = %d\n", dcbSerialParams.BaudRate);
		fprintf(stderr, "		ByteSize = %d\n", dcbSerialParams.ByteSize);
		fprintf(stderr, "		StopBits = %d\n", dcbSerialParams.StopBits);
		fprintf(stderr, "		Parity   = %d\n", dcbSerialParams.Parity);
		fprintf(stderr, "		EOFChar  = %d\n", dcbSerialParams.EofChar);
	}
	else
	{
		fprintf(stderr, "Serial: Set COM DCB Structure Success.\n");
		fprintf(stderr, "		Baudrate = %d\n", dcbSerialParams.BaudRate);
		fprintf(stderr, "		ByteSize = %d\n", dcbSerialParams.ByteSize);
		fprintf(stderr, "		StopBits = %d\n", dcbSerialParams.StopBits);
		fprintf(stderr, "		Parity   = %d\n", dcbSerialParams.Parity);
		fprintf(stderr, "		EOFChar  = %d\n", dcbSerialParams.EofChar);
	}

	COMMTIMEOUTS timeouts = { 0 };

	timeouts.ReadIntervalTimeout	     = args.readIntervalTimeout         ? args.readIntervalTimeout         : SERIAL_DEFAULT_READ_INTERVAL_TIMEOUT;
	timeouts.ReadTotalTimeoutConstant    = args.readTotalTimeoutConstant    ? args.readTotalTimeoutConstant    : SERIAL_DEFAULT_READ_TOTAL_TIMEOUT_CT;
	timeouts.ReadTotalTimeoutMultiplier  = args.readTotalTimeoutMultiplier  ? args.readTotalTimeoutMultiplier  : SERIAL_DEFAULT_READ_TOTAL_MULTIPLIER;
	timeouts.WriteTotalTimeoutConstant   = args.writeTotalTimeoutConstant   ? args.writeTotalTimeoutConstant   : SERIAL_DEFAULT_WRITE_TOTAL_TIMEOUT_CT;
	timeouts.WriteTotalTimeoutMultiplier = args.writeTotalTimeoutMultiplier ? args.writeTotalTimeoutMultiplier : SERIAL_DEFAULT_WRITE_TOTAL_MULTIPLIER;

	//Use this value for checking - but all must be initialized
	if (timeouts.ReadIntervalTimeout)
	{
		status = SetCommTimeouts(hComm, &timeouts);
		if (status == FALSE)
		{
			fprintf(stderr, "Serial: SetCommTimeouts() error. Using no values for timeouts.\n");
		}
		else
		{
			fprintf(stderr, "Serial: Set port timeouts successfull.\n");
		}
	}

	return hComm;
}

BOOL serial_writeBytes(HANDLE hComm, LPCSTR charArray, DWORD NbytesToWrite)
{
	//Check if the handle is in fact valid
	if (hComm == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Serial: HANDLE %u is invalid.\n", (uintptr_t)hComm);
		return FALSE;
	}

	DWORD NbytesWritten = 0;

	//Tx the array to the device
	BOOL status = WriteFile(hComm, charArray, NbytesToWrite, &NbytesWritten, NULL);

	if (status == FALSE)
	{
		fprintf(stderr, "Serial: Tx of data failed: .\n");
		fprintf(stderr, "		Serial port = %u\n", (uintptr_t)hComm);
		fprintf(stderr, "		Serial data = %s\n", charArray);
		fprintf(stderr, "		Error       = %ul\n", GetLastError());
		return FALSE;
	}

	return TRUE;

}

BOOL serial_readBytes(HANDLE hComm, LPTSTR buffer, DWORD bufferSize, LPDWORD readBufferSize)
{
	//Check if the handle is in fact valid
	if (hComm == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Serial: HANDLE %u is invalid.\n", (uintptr_t)hComm);
		return FALSE;
	}

	DWORD dwEventMask = 0;
	char  localSerialBuffer[SERIAL_DEFAULT_READ_BUFFER_SIZE];

	//Receive any byte
	BOOL status = SetCommMask(hComm, EV_RXCHAR);

	if (status == FALSE)
	{
		fprintf(stderr, "Serial: SetCommMask() error. Event %d might be invalid.\n", EV_RXCHAR);
		fprintf(stderr, "Serial: Extended error: %ul\n", GetLastError());
		return FALSE;
	}

	//Wait for the data
	status = WaitCommEvent(hComm, &dwEventMask, NULL);

	if (status == FALSE)
	{
		fprintf(stderr, "Serial: WaitCommEvent() error.\n");
		fprintf(stderr, "Serial: Extended error: %ul\n", GetLastError());
		return FALSE;
	}

	//Start reading the Rxed data
	int i = 0;
	int excess = 0;
	int last = 0;
	while (TRUE)
	{
		char tempChar = 0;
		DWORD NbytesRead = 0;

		status = ReadFile(hComm, &tempChar, sizeof(tempChar), &NbytesRead, NULL);
		if (status == FALSE) break;

		if (i == SERIAL_DEFAULT_READ_BUFFER_SIZE)
		{
			excess++;
			if (tempChar == SERIAL_DEFAULT_READ_RX_EOF) break;
			continue;
		}

		last = i;
		localSerialBuffer[i++] = tempChar;

		if (tempChar == SERIAL_DEFAULT_READ_RX_EOF) break;
	}

	if (i == SERIAL_DEFAULT_READ_BUFFER_SIZE && localSerialBuffer[SERIAL_DEFAULT_READ_BUFFER_SIZE - 1] != SERIAL_DEFAULT_READ_RX_EOF)
	{
		fprintf(stderr, "Serial: Rx of data incomplete.\n");
		fprintf(stderr, "		Read Buffer Size = %d\n", SERIAL_DEFAULT_READ_BUFFER_SIZE);
		fprintf(stderr, "		Received Size    = %d\n", excess + i);
		fprintf(stderr, "		Ignoring Size    = %d\n", excess);
		fprintf(stderr, "		Received Bytes   = %s\n", localSerialBuffer);
		fprintf(stderr, "		Terminated       = %s\n", "False");
		return FALSE;
	}

	if (status == FALSE)
	{
		fprintf(stderr, "Serial: Rx of data failed.\n");
		fprintf(stderr, "Serial: Extended error: %ul\n", GetLastError());
		fprintf(stderr, "		Read Buffer Size = %d\n", SERIAL_DEFAULT_READ_BUFFER_SIZE);
		fprintf(stderr, "		Received Size    = %d\n", excess + i);
		fprintf(stderr, "		Ignoring Size    = %d\n", excess);
		fprintf(stderr, "		Received Bytes   = %s\n", localSerialBuffer);
		fprintf(stderr, "		Terminated       = %s\n", localSerialBuffer[last] == '\n' ? "True" : "False");
		return FALSE;
	}

	//Replace the terminator char for a string terminator
	localSerialBuffer[last] = '\0';

	//Copy the data to the argument buffer
	DWORD localBufferSize = strlen(localSerialBuffer);

	if (bufferSize >= localBufferSize)
	{
		strcpy_s(buffer, localBufferSize, localSerialBuffer);
		*readBufferSize = localBufferSize;
	}
	else
	{
		fprintf(stderr, "Serial: Rx copy of data failed.\n");
		fprintf(stderr, "Serial: Size of user buffer is too small.\n");
		fprintf(stderr, "		Read Buffer Size = %d\n", SERIAL_DEFAULT_READ_BUFFER_SIZE);
		fprintf(stderr, "		User Buffer Size = %d\n", bufferSize);
		return FALSE;
	}

	return TRUE;
}

BOOL serial_closeHandle(HANDLE hComm)
{
	return CloseHandle(hComm);
}
