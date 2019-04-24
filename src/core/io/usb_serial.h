#ifndef USB_SERIAL_H_
#define USB_SERIAL_H_
#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

	typedef struct _serialInitArgs
	{
		DWORD baudRate;
		BYTE byteSize;
		BYTE stopBits;
		BYTE parity;

		CHAR eofChar;

		DWORD readIntervalTimeout;
		DWORD readTotalTimeoutConstant;
		DWORD readTotalTimeoutMultiplier;
		DWORD writeTotalTimeoutConstant;
		DWORD writeTotalTimeoutMultiplier;
	}SerialArgs;

	HANDLE serial_initHandle(LPCSTR portName, DWORD rwAccess, SerialArgs args);
	BOOL   serial_writeBytes(HANDLE hComm, LPCSTR charArray, DWORD NbytesToWrite);
	BOOL   serial_readBytes(HANDLE hComm, LPTSTR buffer, DWORD bufferSize, LPDWORD readBufferSize);
	BOOL   serial_closeHandle(HANDLE hComm);

	/* Implementation specific functions */
	/* (...) */


#ifdef __cplusplus
}
#endif

#endif /* USB_SERIAL_H_ */