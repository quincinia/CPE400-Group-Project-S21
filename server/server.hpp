//
// CPE 400 Group Project Spring 2021
// Jacob Gayban, Ernesto Bernardo, Ritik Bhardwaj
//
// This file describes the Server (receiver) class
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#define TCP_PORT 1234
#define UDP_PORT 1235

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

    }

    void udp_accept()
    {

    }

    // the receive functions may need different return types
    void receive_metadata(std::fstream& file)
    {

    }

    void receive_packet(std::fstream& file)
    {

    }

    void write_file(std::fstream& file)
    {
        
    }
};