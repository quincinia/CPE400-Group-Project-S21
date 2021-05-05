//
// CPE 400 Group Project Spring 2021
// Jacob Gayban, Ernesto Bernardo, Ritik Bhardwaj
//
// This is the program for the RECEIVER of the file.
//
// Object-oriented version.
//

#include "server.hpp"
#include <sstream>

int main(int argc, char* argv[])
{
    try
    {
        // default constructor automatically opens ports
        Server server;

        // wait for TCP connection
        server.tcp_accept();

        // receive metadata from client
        std::vector<char> metadata;
        metadata = server.receive_metadata();

        // create file from metadata
        std::stringstream data_stream;
        std::ofstream file;
        if (metadata.size() == 0)
        {
            std::cerr << "Metadata fail!" << std::endl;
            return 1;
        }
        else
        {
            std::string file_name;
            for (char c : metadata)
                data_stream << c;
            data_stream >> file_name;
            file.open(file_name, std::ios_base::out | std::ios_base::binary);
        }

        // buffer for receiving data packets
        std::vector<char> packet;

        do
        {
            std::string msg;

            // receive UDP message
            if (server.receive_packet(packet))
            {
                // packet is good, send ACK for next one
                msg = server.generate_ack();
                server.tcp_send(msg);

                // write data into file
                server.write_file(file, packet);
            }
            else
            {
                // wrong packet was received, send ACK for prev one
                msg = server.generate_ack();
                server.tcp_send(msg);
            }
        } 
        while (!server.transfer_finished());

        file.close();    
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}