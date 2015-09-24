A command line program to read serial data from an Arduino.

Usage:

    -p <port>    - Specify the port to access the Arduino (COM1, COM2, etc.)

    -l <size>    - Specify the maximum size in bytes of data to retrieve

    -f <path>    - Write serial output to a file specified by <path>

    -t <time>    - Specifies how long to capture serial data in seconds

    -d <time>    - Specifies a millisecond delay (per loop) to capture the serial data

    -q           - Quiet mode, serial output will not be displayed in the console

    -n           - Newline mode, seperates serial data by newlines

You may need to the program run as an administrator for the file ouput to work properly.

Written in VS2015 using the standard templete libary and various Windows functions.
Requires the VS2015 redist package.
