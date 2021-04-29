//
// CPE 400 Group Project Spring 2021
// Jacob Gayban, Ernesto Bernardo, Ritik Bhardwaj
//
// This is the program for the SENDER of the file.
//
// Object-oriented version.
//

#include "client.hpp"

int main(int argc, char* argv[])
{
    try
    {
        Client client;
        std::fstream file;

        if (argc != 3)
        {
            std::cerr << "Usage: client <host> <filename>" << std::endl;
            return 1;
        }

        // connect to TCP server
        client.tcp_connect(argv[1]);

        // connect to UDP server
        client.udp_connect(argv[1]);

        // create metadata
        std::string metadata = client.generate_metadata(file, argv[2]);

        // send metadata
        client.tcp_send(metadata);

        // open file for packet generation
        file.open(argv[2]);
        
        // vector to hold the packet
        std::vector<char> packet;
        client.generate_packet(file, packet);

        // send packet over UDP
        client.udp_send(packet);

    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
    }
    return 0;
}