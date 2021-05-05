# CPE400-Group-Project-S21
Jacob Gayban, Ernesto Bernardo, Ritik Bhardwaj

Note: this project requires use of the [boost libraries](https://www.boost.org/users/download/) in order to work. This project was originally compiled using version 1.75.0.
When compiling, the "lws2_32" flag may need to be set.

This project was built and tested on Windows using g++, with the following arguments:
`g++ -g <file.cpp> -o <file.exe> -lws2_32`

## Usage
The server requires no arguments, and can be executed normally: `./oo_server`   
Note that the server program should be run **before** the client.

The client takes 2 arguments:
1. The IP address of the server (use `127.0.0.1` if the server is local, "localhost" doesn't work)
2. The name the file to send (the file **must be in the same directory** as the executable)

For best results, keep the client and server executables in **separate directories**, and place the files to send in the same directory as that of the client executable.
