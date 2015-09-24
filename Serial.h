#ifndef SERIAL_H
#define SERIAL_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class Serial
{
private:
	bool connected;
	HANDLE hSerial;
	COMSTAT status;
	DWORD errors;

public:
	Serial(char *portName);
	~Serial();
	
	int ReadData(char *buffer, unsigned int nbChar);
	bool WriteData(char *buffer, unsigned int nbChar);

	bool IsConnected();

};

#endif