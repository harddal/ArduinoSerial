#include "Serial.h"

#include <chrono>
#include <thread>

Serial::Serial(char *portName, int baudrate)
{
	this->connected = false;

	this->hSerial = CreateFile(portName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (this->hSerial == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND) 
		{
			printf("\nERROR: Handle was not attached, %s not available\n\n", portName);
		}
		else
		{
			printf("\nERROR: Unknown error\n\n");
		}
	}
	else
	{
		DCB dcbSerialParams = { 0 };

		if (!GetCommState(this->hSerial, &dcbSerialParams))
		{
			printf("\nERROR: Failed to get current serial parameters\n\n");
		}
		else
		{
			dcbSerialParams.BaudRate = baudrate;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;
			dcbSerialParams.fDtrControl = DTR_CONTROL_DISABLE;

			if (!SetCommState(hSerial, &dcbSerialParams))
			{
				printf("\nALERT: Could not set Serial Port parameters\n\n");
			}
			else
			{
				this->connected = true;
				PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
			}
		}
	}
}

Serial::~Serial()
{
	if (this->connected)
	{
		this->connected = false;

		CloseHandle(this->hSerial);
	}
}

int Serial::ReadData(char *buffer, unsigned int nbChar)
{
	DWORD bytesRead;
	unsigned int toRead;

	ClearCommError(this->hSerial, &this->errors, &this->status);

	if (this->status.cbInQue>0)
	{
		if (this->status.cbInQue>nbChar)
		{
			toRead = nbChar;
		}
		else
		{
			toRead = this->status.cbInQue;
		}

		if (ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL) && bytesRead != 0)
		{
			return bytesRead;
		}

	}

	return -1;

}

bool Serial::WriteData(char *buffer, unsigned int nbChar)
{
	DWORD bytesSend;

	if (!WriteFile(this->hSerial, (void *)buffer, nbChar, &bytesSend, 0))
	{
		ClearCommError(this->hSerial, &this->errors, &this->status);

		return false;
	}
	else
		return true;
}

bool Serial::IsConnected()
{
	return this->connected;
}
