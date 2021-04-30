//
// CPE 400 Group Project Spring 2021
// Jacob Gayban, Ernesto Bernardo, Ritik Bhardwaj
//
// This file describes the Server (receiver) class
//
#ifndef SERVER
#define SERVER

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

class Server
{
private:
    // all asio programs need an io_context
    boost::asio::io_context io_context;

    // TCP stuff
    tcp::socket tcp_socket;
    tcp::acceptor acceptor;

    // UDP stuff
    udp::socket udp_socket;
    udp::endpoint remote_endpoint;

    // keep track of sequence numbers
    std::uint32_t sequence_num = 0;

    // flag to indicate that we should not be expecting more packets
    bool final_packet_received = false;

public:
    Server() :
        tcp_socket(io_context),
        acceptor(io_context, tcp::endpoint(tcp::v4(), TCP_PORT)),
        udp_socket(io_context, udp::endpoint(udp::v4(), UDP_PORT))
    {

    }

    void tcp_accept()
    {
        std::cout << "Accepting TCP connections... ";
        acceptor.accept(tcp_socket);
        std::cout << "connected!" << std::endl;
    }

    void udp_accept()
    {
        // the UDP socket is already set up and we don't need
        // to accept connections so this function is unneeded?
    }

    // the receive functions may need different return types
    std::vector<char> receive_metadata(/*std::fstream& file*/)
    {
        // boost::asio::streambuf receive_buffer;
        std::vector<char> metadata(PACKET_SIZE);
        boost::system::error_code error;
        
        std::cout << "Receiving metadata..." << std::endl;

        // std::size_t len = boost::asio::read(tcp_socket, boost::asio::buffer(metadata), boost::asio::transfer_all(), error);

        // std::size_t len = tcp_socket.receive(boost::asio::buffer(metadata));

        std::size_t len = tcp_socket.read_some(boost::asio::buffer(metadata), error);

        if (error && error != boost::asio::error::eof)
        {
            std::cerr << "Receive failed: " << error.message() << std::endl;
        }
        else
        {
            // const char *data = boost::asio::buffer_cast<const char *>(receive_buffer.data());
            metadata.resize(len);
            std::cout << "Received: ";
            for (char c : metadata)
                std::cout << c;
            std::cout << std::endl;
        }
        return metadata;
    }

    bool verify_sequence_num(std::vector<char>& packet)
    {
        // check if the packet has the sequence number we are expecting
        // if the packet is the last packet, then it will be accepted
        sequence_num_translator translator{sequence_num};
        return (translator.translate[0] == packet[0]  &&
                translator.translate[1] == packet[1]  &&
                translator.translate[2] == packet[2]  &&
                translator.translate[3] == packet[3]) 
                ||
               (packet[0] == (char) 0xFF &&
                packet[1] == (char) 0xFF &&
                packet[2] == (char) 0xFF &&
                packet[3] == (char) 0xFF);
    }

    bool receive_packet(std::vector<char>& packet)
    {
        std::vector<char> buffer(PACKET_SIZE);
        boost::system::error_code error;
        std::cout << "Waiting for UDP data... ";

        // read() returns the number of bytes read, aka the length of the vector/array
        std::size_t len = udp_socket.receive(boost::asio::buffer(buffer));
        if (error && error != boost::asio::error::eof)
        {
            std::cerr << "Receive failed: " << error.message() << std::endl;
            return false;
        }

        // std::cout << "received!" << std::endl;

        if (verify_sequence_num(buffer))
        {
            // if the packet is the last one, we should not accept any more
            if (buffer[0] == (char)0xFF &&
                buffer[1] == (char)0xFF &&
                buffer[2] == (char)0xFF &&
                buffer[3] == (char)0xFF)
            {
                final_packet_received = true;
            }
            else
            {
                // otherwise, we will update our expected sequence
                sequence_num++;
            }
        }
        else
        {
            // if the packet is out of order, we won't accept it
            return false;
        }
        
        std::cout << "success!" << std::endl;
        std::cout << "Size of packet: " << len << std::endl;
        std::cout << "Received data: ";
        for (int i = 4; i < len; i++)
            std::cout << buffer[i];
        std::cout << std::endl;

        // save data into packet
        buffer.resize(len);
        packet = buffer;

        return true;
    }

    // creates an acknowledgement message based
    // off the current expected sequence number
    // Packet format: <4-character sequence number> + <space> + <human-readable message (optional)>
    std::string generate_ack()
    {
        sequence_num_translator translator{sequence_num};
        if (final_packet_received)
        {
            return std::string() + ((char)0xFF) + ((char)0xFF) + ((char)0xFF) + ((char)0xFF) + "\tFinal packet was received!";
        }
        else 
        {
            return std::string() + translator.translate[0] + translator.translate[1] + translator.translate[2] + translator.translate[3] + "\tExpecting next packet to have sequence: " + std::to_string(sequence_num);
        }
    }
    // synchronous send
    // messages will be generated outside of this function
    // see above for message format
    bool tcp_send(std::string message)
    {
        std::cout << "Sending response: " << message << std::endl;

        boost::system::error_code error;
        boost::asio::write(tcp_socket, boost::asio::buffer(message), error);
        if (!error)
        {
            std::cout << "Success!" << std::endl;
            return true;
        }
        else
        {
            std::cerr << "Failed: " << error.message() << std::endl;
            return false;
        }
    }

    // requires an opened file
    // vector must have the standard packet format (first 4 bytes reserved)
    void write_file(std::ofstream& file, std::vector<char>& data)
    {
        // add error checking code?
        for (int i = 4; i < data.size(); i++)
            file << data[i];
    }

    // so we know when to stop looping
    bool transfer_finished()
    {
        return final_packet_received;
    }
};

#endif