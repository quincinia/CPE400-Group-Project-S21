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

    }

    // the receive functions may need different return types
    void receive_metadata(/*std::fstream& file*/)
    {
        boost::asio::streambuf receive_buffer;
        boost::system::error_code error;
        boost::asio::read(tcp_socket, receive_buffer, boost::asio::transfer_all(), error);
        if (error && error != boost::asio::error::eof)
        {
            std::cout << "Receive failed: " << error.message() << std::endl;
        }
        else
        {
            const char *data = boost::asio::buffer_cast<const char *>(receive_buffer.data());
            std::cout << "Received: " << data << std::endl;
        }
    }

    void receive_packet(std::fstream& file)
    {

    }

    void write_file(std::fstream& file)
    {

    }
};

#endif