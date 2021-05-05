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

    // Returns the file size for an existing file
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

    // signals when we have nothing to send
    bool final_packet_sent = false;

public:
    // initialize sockets
    Client() :
        tcp_socket(io_context),
        udp_socket(io_context)
    {
        
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
        return std::string(file_name) + " " + std::to_string(file_size);
    }

    // file must already be open when this is called
    // Packet format: <4-character sequence number> + <up to 1020 data characters>
    bool generate_packet(std::fstream& file, std::vector<char>& buf)
    {
        std::vector<char> temp(4);

        // input data
        for (int i = 0; i < PACKET_SIZE-4; i++)
        {
            char c;

            // grab characters until failure or end of loop
            if (file.get(c))
            {                
                temp.push_back(c);
            }
            else
            {
                std::cout << "Early break!, i = " << i << std::endl;
                break;
            }
        }

        std::cout << "Generated packet: ";

        // input sequence numbers
        sequence_num_translator translator{sequence_num++};
        if (file.eof())
        {
            // maxed out sequence number
            temp[0] = temp[1] = temp[2] = temp[3] = (char) 0xFF;
            std::cout << "[final]" << std::endl;
            final_packet_sent = true;
        }
        else 
        {
            // translated sequence number
            temp[0] = translator.translate[0];
            temp[1] = translator.translate[1];
            temp[2] = translator.translate[2];
            temp[3] = translator.translate[3];
            std::cout << (sequence_num-1) << std::endl;
        }

        // in case the new data is smaller than the last packet, this will
        // remove the extra data that isn't part of the current packet
        buf = temp;
        // normally we would put this inside an if-statement, however we
        // are not expecting anything to go wrong with reading from the file

        std::cout << "Size of data packet: " << buf.size() << std::endl;

        // this might not be needed because the state of the file can be 
        // checked outside of this function
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

    // packets will be sent using UDP, this function is obsolete
    void tcp_send(std::vector<char>& packet)
    {

    }

    // packets must be formatted correctly before sending
    bool udp_send(std::vector<char>& packet)
    {
        std::cout << "Sending packet... ";
        boost::system::error_code error;
        udp_socket.send(boost::asio::buffer(packet));
        if (!error)
        {
            std::cout << "success!" << std::endl;

            // std::cout << "Packet data: ";
            // for (int i = 4; i < packet.size(); i++)
            //     std::cout << packet[i];
            // std::cout << std::endl;

            return true;
        }
        else
        {
            std::cerr << "failed: " << error.message() << std::endl;
            return false;
        }
    }

    bool tcp_read(std::vector<char>& msg)
    {
        std::vector<char> buffer(PACKET_SIZE);
        boost::system::error_code error;
        std::cout << "Waiting for server message... ";

        // read functions will return the number of bytes read
        std::size_t len = tcp_socket.read_some(boost::asio::buffer(buffer), error);

        if (error && error != boost::asio::error::eof)
        {
            std::cerr << "Receive failed: " << error.message() << std::endl;
            return false;
        }
        else
        {
            std::cout << "received!" << std::endl;
            buffer.resize(len);
            msg = buffer;

            std::cout << "Message: ";
            for (char c : buffer)
                std::cout << c;
            std::cout << std::endl;
            return true;
        }
    }

    // the server will only send its messages through the TCP channel
    void udp_read(std::vector<char> &msg)
    {

    }

    // Check if the sequence number the receiver is expecting
    // is the same one as the one we are preparing to send
    // If we sent the last packet, then we don't care
    // Returns true if we are on track
    bool confirm_ack(std::vector<char>& ack)
    {
        sequence_num_translator translator;
        translator.translate[0] = ack[0];
        translator.translate[1] = ack[1];
        translator.translate[2] = ack[2];
        translator.translate[3] = ack[3];
        return (translator.sequence_num == sequence_num) || final_packet_sent;
    }
};
    
#endif