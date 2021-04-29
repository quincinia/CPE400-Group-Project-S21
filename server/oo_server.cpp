//
// CPE 400 Group Project Spring 2021
// Jacob Gayban, Ernesto Bernardo, Ritik Bhardwaj
//
// This is the program for the RECEIVER of the file.
//
// Object-oriented version.
//

#include "server.hpp"

int main(int argc, char* argv[])
{
    try
    {
        // default constructor automatically opens ports
        Server server;

        // wait for TCP connection
        server.tcp_accept();

        // receive metadata from client
        server.receive_metadata();

        // buffer for receiving data packets
        std::vector<char> packet;

        // TEST: receive UDP message
        server.receive_packet(packet);
        
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}