#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include "Serial.h"
#include "Timer.h"

bool cmdOptionExists(char **begin, char **end, const std::string &option);
char* getCmdOption(char **begin, char **end, const std::string &option);
std::string programInfo();

int main(int argc, char *argv[])
{
	std::ofstream f_out;
	
	bool quietMode = false, fileOutMode = false, timeOut = false;
	char data[256] = "";
	char *port = "", *input = "", *filename = "";
	int length = 256, exeTime = 0;
	

	if (argc < 2)
	{
		std::cout << programInfo();
		return 0;
	}

	if (cmdOptionExists(argv, argv + argc, "-p"))
		port = getCmdOption(argv, argv + argc, "-p");
	else
	{
		std::cout << "\nNo port specified, use -p <port> to specify a port\n\n";
		return 0;
	}

	if (cmdOptionExists(argv, argv + argc, "-l"))
	{
		length = atoi(getCmdOption(argv, argv + argc, "-l"));
	}

	if (cmdOptionExists(argv, argv + argc, "-f"))
	{
		fileOutMode = true;

		filename = getCmdOption(argv, argv + argc, "-f");

		f_out.open(filename);
		
		if (!f_out.is_open())
		{
			std::cout << "\nFailed to open file " << filename << ", make sure to run this program as an administrator\n\n";
			return 0;
		}
	}

	if (cmdOptionExists(argv, argv + argc, "-t"))
	{
		timeOut = true;
		exeTime = atoi(getCmdOption(argv, argv + argc, "-t"));
	}

	if (cmdOptionExists(argv, argv + argc, "-q"))
	{
		quietMode = true;
	}

	Serial* SP = new Serial(port);

	if (!SP->IsConnected())
	{
		std::cout << "\nDevice on " << port << " not detected\n\n";
		return 0;
	}
	else
	{
		std::cout << "\nDevice on " << port << " detected, reading serial data...\n\n";
	}
	
	Timer timer(true);
	while (SP->IsConnected())
	{
		int r = SP->ReadData(data, length);

		if (r > 0 && !quietMode)
			printf("%s\n", data);

		if (r > 0 && fileOutMode)
			f_out << data << "\n";

		if (timer.Elapsed().count() > exeTime && timeOut)
			break;
	}

	f_out.close();

	if (timeOut)
		std::cout << "Read serial data for " << exeTime << " seconds\n";
	if (fileOutMode)
		std::cout << "Wrote serial data to file " << filename << "\n";

	std::cout << "\n";

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
		"\n Arduino Serial Communication Tool - Created by Dallas Hardwicke\n\n"
		" -p <port>    - Specify the port to access the Arduino\n"
		" -l <size>    - Specify the maximum size in bytes of data to retrieve\n"
		" -f <path>    - Write serial output to a file specified by <path>\n"
		" -t <time>    - Specifies how long to capture serial data in seconds\n"
		" -q           - Quiet mode, serial output will not be displayed\n"
		"\n";
}
