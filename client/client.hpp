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
    udp::socket udp_socket;
    //udp::resolver resolver;
    //udp::endpoint receiver_endpoint;
    //udp::endpoint sender_endpoint;

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
        tcp_socket(io_context),
        udp_socket(io_context)
    {
        // handle UDP stuff later
    }

    void tcp_connect(char* address) 
    {
        std::cout << "Connecting to TCP channel... ";
        tcp_socket.connect(tcp::endpoint(boost::asio::ip::make_address(address), TCP_PORT));
        std::cout << "connected!" << std::endl;
    }

    void udp_connect(char* address) 
    {
        std::cout << "Finding UDP endpoint... ";
        udp_socket.connect(udp::endpoint(boost::asio::ip::make_address(address), UDP_PORT));
        std::cout << "found!" << std::endl;
    }

    std::string generate_metadata(std::fstream& file, char* file_name) 
    {
        long file_size = get_file_size(file, file_name);
        if (file_size == -1) {
            throw std::runtime_error("File size error");
        }
        return std::to_string(file_size) + " " + std::string(file_name);
    }

    // file must already be open when this is called
    // vector must be preallocated as well
    bool generate_packet(std::fstream& file, std::vector<char>& buf)
    {
        std::vector<char> temp(4);

        // input data
        for (int i = 0; i < PACKET_SIZE-4; i++)
        {
            char c;

            // grab characters until failure or end of loop
            if (file.get(c))
                temp.push_back(c);
            else
                break;
        }

        // input sequence numbers
        translate_sequence_num translator{sequence_num++};
        if (file.eof())
        {
            // maxed out sequence number
            temp[0] = temp[1] = temp[2] = temp[3] = (char) 0xFF;
        }
        else 
        {
            // translated sequence number
            temp[0] = translator.translate[0];
            temp[1] = translator.translate[1];
            temp[2] = translator.translate[2];
            temp[3] = translator.translate[3];
        }

        // in case the new data is smaller than the last packet, this will
        // remove the extra data that isn't part of the current packet
        buf = temp;

        std::cout << "Size of data packet: " << buf.size() << std::endl;

        // this not be needed because the state of the file can be checked outside of this function
        return file.good();
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