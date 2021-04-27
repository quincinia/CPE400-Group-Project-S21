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

        // connect to server
        client.tcp_connect(argv[1]);

        // create metadata
        std::string metadata = client.generate_metadata(file, argv[2]);

        // send metadata
        client.tcp_send(metadata);

    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << '\n';
    }
    return 0;
}