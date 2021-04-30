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
        file.open(argv[2], std::ios_base::in | std::ios_base::binary);
        
        // packet generation loop
        std::vector<char> packet, ack;
        while (!file.eof())
        {
            // generate and send packet over
            client.generate_packet(file, packet);
            client.udp_send(packet);

            // wait for ack message
            client.tcp_read(ack);

            // confirm ack, resend if needed
            // for GBN or other pipelined protocols, the
            // ack-checking will need to be modified
            while (!client.confirm_ack(ack))
                client.udp_send(packet);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
    }
    return 0;
}