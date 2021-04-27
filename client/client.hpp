//
// CPE 400 Group Project Spring 2021
// Jacob Gayban, Ernesto Bernardo, Ritik Bhardwaj
//
// This file describes the Client (sender) class
//
#ifndef CLIENT
#define CLIENT

#include "../include/shared.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class Client 
{
private:
    // all asio programs need an io_context
    boost::asio::io_context io_context;

    // for TCP connections
    tcp::socket tcp_socket;

    // for UDP connections (some of these may not be needed)
    // udp::resolver resolver;
    // udp::endpoint receiver_endpoint;
    // udp::endpoint sender_endpoint;
    // udp::socket udp_socket;

    /**
     * @brief Returns the file size for a
     * @param file The file to check the size
     * @param file_name Name of the file to check
     */
    long get_file_size(std::fstream &file, char *file_name)
    {
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
        return (end - begin);
    }

    // to keep track of sequence numbers
    std::uint32_t sequence_num = 0;

public:
    Client() :
        tcp_socket(io_context)
    {
        // handle UDP stuff later
    }

    void tcp_connect(char* address) 
    {
        std::cout << "Connecting to TCP channel... ";
        tcp_socket.connect(tcp::endpoint(boost::asio::ip::make_address(address), TCP_PORT));
        std::cout << "connected!" << std::endl;
    }

    void udp_connect() 
    {
        std::cout << "Connecting to UDP channel... ";
        
    }

    std::string generate_metadata(std::fstream& file, char* file_name) 
    {
        long file_size = get_file_size(file, file_name);
        if (file_size == -1) {
            throw std::runtime_error("File size error");
        }
        return std::to_string(file_size) + " " + std::string(file_name);
    }

    bool generate_packet(std::fstream& file, std::vector<char>& buf)
    {

    }

    // synchronous (blocking) send
    bool tcp_send(std::string& message)
    {
        std::cout << "Metadata: " << message << std::endl;
        std::cout << "Sending metadata... ";
        boost::system::error_code error;
        boost::asio::write(tcp_socket, boost::asio::buffer(message), error);
        if (!error)
        {
            std::cout << "success!" << std::endl;
            return true;
        }
        else
        {
            std::cerr << "failed: " << error.message() << std::endl;
            return false;
        }
    }

    // actually packets will be sent using TCP, so this might be obsolete
    void tcp_send(std::vector<char>& packet)
    {

    }

    void udp_send(std::vector<char>& packet)
    {

    }

    // return types/parameters may change
    void tcp_read(std::vector<char>& msg)
    {

    }

    void udp_read(std::vector<char> &msg)
    {

    }
};
    
#endif