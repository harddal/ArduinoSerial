A command line program to read serial data from an Arduino. The baud rate is set to 9600 unless you specifiy it with '-b'.

Usage:

    -r              - Start the program in read mode

    -w              - Start the program in write mode
    
    -b <rate>       - Set the connections baud rate (defaulty 9600)

    -p <port>       - Specify the port to access the Arduino

    -l <size>       - Specify the maximum size in bytes of data to read/write

    -f <path>       - Write serial output to a file specified by <path>

    -t <time>       - Specifies how long to capture serial data in seconds

    -d <time>       - Specifies a millisecond delay (per loop) to capture the serial data

    -data <char>    - Char data to write, must be aligned with -l (1 byte == 1 char)

    -q              - Quiet mode, serial output will not be displayed

    -n              - Newline mode, seperate serial data by newlines

    -s              - Spam mode, sends as many bytes as possible, infinitely

    -c              - Capture mode, waits until it recieves one byte of data and halts

You may need to the program run as an administrator for the file ouput to work properly.

Written in VS2015 using the standard templete libary and various Windows functions.
Requires the VS2015 redist package.
