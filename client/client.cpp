//
// CPE 400 Group Project Spring 2021
// Jacob Gayban, Ernesto Bernardo, Ritik Bhardwaj
//
// This is the program for the SENDER of the file.
// 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

union translate_sequence_num
{
    std::uint32_t sequence_num;
    char translate[4];
};

/**
 * @brief Returns the file size for a
 * @param file The file to check the size
 * @param file_name Name of the file to check
 */
long get_file_size(std::fstream& file, char* file_name) {
    using namespace std;
    file.open(file_name);
    if (!file.is_open())
    {
        return -1;
    }
    long begin, end;
    begin = file.tellg();
    file.seekg(0, ios::end);
    end = file.tellg();
    file.close();
    return (end-begin);
}

int main(int argc, char *argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: client <host> <filename>" << std::endl;
            return 1;
        }

        std::fstream file;        
        long file_size = get_file_size(file, argv[2]);
        file.open(argv[2]);
        if (file_size == -1 || !file.is_open())
        {
            std::cerr << "Problem opening file" << std::endl;
            return 1;
        }


        // all asio programs need an io_context
        boost::asio::io_context io_context;

        tcp::socket tcp_socket(io_context);

        // using random port number
        // error will be thrown if connection fails
        std::cout << "Connecting to server..." << std::endl;

        // note: make_address does not work with "localhost", use 127.0.0.1
        tcp_socket.connect(tcp::endpoint(boost::asio::ip::make_address(argv[1]), 1234)); 

        // metadata string
        std::cout << "Creating metadata..." << std::endl;
        std::string metadata = std::to_string(file_size) + " " + std::string(argv[2]);

        // send metadata over
        std::cout << "Sending metadata..." << std::endl;
        boost::system::error_code error;
        boost::asio::write(tcp_socket, boost::asio::buffer(metadata), error);

        if (!error) 
        {
            std::cout << "Sent metadata: " << metadata << std::endl;
        }
        else
        {
            std::cerr << "Send failed: " << error.message() << std::endl;
            return 1;
        }

        

    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}