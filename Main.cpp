#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <thread>

#include "Serial.h"
#include "Timer.h"

#define printm(c) std::cout << c

bool cmdOptionExists(char **begin, char **end, const std::string &option);
char* getCmdOption(char **begin, char **end, const std::string &option);
std::string programInfo();

int main(int argc, char *argv[])
{
	std::ofstream f_out;
	
	bool quietMode = false, fileOutMode = false, 
		timeOut = false, newLineMode = false, 
		loopDelay = false, writeMode = false, 
		spamMode = false, captureMode = false, 
		captureFlag = false;

	char data[256] = "";
	char *port = "", *input = "", 
		*filename = "", *captureChar = "";

	int length = 1, exeTime = 0, 
		delayTime = 0, bytesRecieved = 0, 
		bytesTransmitted = 0;
	

	if (argc < 2)
	{
		std::cout << programInfo();
		return 0;
	}

	if (cmdOptionExists(argv, argv + argc, "-r"))
	{
		writeMode = false;
	}
	else if (cmdOptionExists(argv, argv + argc, "-w"))
	{
		writeMode = true;
	}
	else
	{
		std::cout << "\n Read/Write mode not specified, use -r or -w\n\n";
		return 0;
	}

	if (cmdOptionExists(argv, argv + argc, "-p"))
		port = getCmdOption(argv, argv + argc, "-p");
	else
	{
		std::cout << "\n  No port specified, use -p <port> to specify a port\n\n";
		return 0;
	}

	if (cmdOptionExists(argv, argv + argc, "-l"))
	{
		length = atoi(getCmdOption(argv, argv + argc, "-l"));
	}

	if (cmdOptionExists(argv, argv + argc, "-f"))
	{
		if (!writeMode)
		{
			fileOutMode = true;

			filename = getCmdOption(argv, argv + argc, "-f");

			f_out.open(filename);

			if (!f_out.is_open())
			{
				std::cout << "\n  Failed to open file " << filename << ", make sure to run this program as an administrator\n\n";
				return 0;
			}
		}
		else
		{
			std::cout << "\n  Cannot output to file in write mode\n\n";
			return 0;
		}
	}

	if (cmdOptionExists(argv, argv + argc, "-t"))
	{
		timeOut = true;
		exeTime = atoi(getCmdOption(argv, argv + argc, "-t"));
	}

	if (cmdOptionExists(argv, argv + argc, "-d"))
	{
		loopDelay = true;
		delayTime = atoi(getCmdOption(argv, argv + argc, "-d"));
	}

	if (cmdOptionExists(argv, argv + argc, "-data"))
	{
		char *_data = getCmdOption(argv, argv + argc, "-data");
		strcpy(data, _data);
		bytesTransmitted = strlen(_data);
	}

	if (cmdOptionExists(argv, argv + argc, "-q"))
	{
		quietMode = true;

		if (writeMode)
			std::cout << "\n  Warning: -q has no effect in write (-w) mode\n\n";
	}

	if (cmdOptionExists(argv, argv + argc, "-n"))
	{
		newLineMode = true;

		if (writeMode)
			std::cout << "\n  Warning: -n has no effect in write (-w) mode\n\n";
	}

	if (cmdOptionExists(argv, argv + argc, "-s"))
	{
		spamMode = true;

		if (!writeMode)
			std::cout << "\n  Warning: -s has no effect in read (-r) mode\n\n";
	}

	if (cmdOptionExists(argv, argv + argc, "-c"))
	{
		if (!writeMode)
		{
			captureMode = true;
		}
		
		if (writeMode)
			std::cout << "\n  Warning: -c has no effect in write (-w) mode\n\n";
		if (timeOut)
			std::cout << "\n  Warning: -c has no effect in timeout (-t) mode\n\n";
	}

	Serial* SP = new Serial(port);

	if (!SP->IsConnected())
	{
		std::cout << "\n  Device on " << port << " not detected\n\n";
		return 0;
	}
	else
	{
		std::cout << "\n  Device on " << port << " detected, " << (writeMode ? "writing" : "reading") << " serial data...\n\n";
	}
	
	printf("\n");

	Timer timer(true);
	while (SP->IsConnected())
	{
		if (!writeMode)
		{
			int r = SP->ReadData(data, length);

			if (r > 0)
				bytesRecieved += r;

			if (r > 0 && !quietMode)
				newLineMode ? printf("    %s\n", data) : printf("%s", data);

			if (r > 0 && fileOutMode)
				newLineMode ? f_out << data << "\n" : f_out << data;

			if (captureMode && r > 0)
			{
				newLineMode ? std::cout << "\n" : std::cout << "\n\n";
				break;
			}

			if (timer.Elapsed().count() > exeTime && timeOut)
			{
				newLineMode ? std::cout << "\n" : std::cout << "\n\n";
				break;
			}

			if (loopDelay)
				std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
		}
		else if (writeMode)
		{
			if (!timeOut && !spamMode)
			{
				SP->WriteData(data, length);
				break;
			}
			else
				SP->WriteData(data, length);

			if (timer.Elapsed().count() > exeTime && timeOut && !spamMode)
			{
				break;
			}

			if (loopDelay)
				std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
		}
	}

	f_out.close();

	if (timeOut)
	{
		if (!writeMode)
			std::cout << "  Received " << bytesRecieved << (bytesRecieved > 1 ? " bytes" : " byte") << " of serial data for " << exeTime << (exeTime > 1 ? " seconds\n" : " second\n");
		else
			std::cout << "  Transmitted " << bytesTransmitted << (bytesTransmitted > 1 ? " bytes" : " byte") << " of serial data for " << exeTime << (exeTime > 1 ? " seconds\n" : " second\n");
	}
	else
		if (!writeMode)
			std::cout << "  Received " << bytesRecieved << (bytesRecieved > 1 ? " bytes" : " byte") << " of serial data\n";
		else
			std::cout << "  Transmitted " << bytesTransmitted << (bytesTransmitted > 1 ? " bytes" : " byte") << " of serial data\n";
		
	if (fileOutMode)
		std::cout << "  Wrote captured serial data to file " << filename << "\n";

	printf("\n");

	return 0;
}

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
	return std::find(begin, end, option) != end;
}

char* getCmdOption(char **begin, char **end, const std::string &option)
{
	char **itr = std::find(begin, end, option);

	if (itr != end && ++itr != end)
	{
		return *itr;
	}

	return 0;
}

std::string programInfo()
{
	return 
		"\n  Arduino Serial Communication Tool - Created by Dallas Hardwicke\n\n"
		"  -r              - Start the program in read mode\n"
		"  -w              - Start the program in write mode\n"
		"  -p <port>       - Specify the port to access the Arduino\n"
		"  -l <size>       - Specify the maximum size in bytes of data to read/write\n"
		"  -f <path>       - Write serial output to a file specified by <path>\n"
		"  -t <time>       - Specifies how long to capture serial data in seconds\n"
		"  -d <time>       - Specifies a millisecond delay (per loop) to capture the serial data\n"
		"  -data <char>    - Char data to write, must be aligned with -l (1 byte == 1 char)\n"
		"  -q              - Quiet mode, serial output will not be displayed\n"
		"  -n              - Newline mode, seperate serial data by newlines\n"
		"  -s              - Spam mode, sends as many bytes as possible, infinitely\n"
		"  -c              - Capture mode, waits until it recieves one byte of data and halts\n"
		"\n";
}
