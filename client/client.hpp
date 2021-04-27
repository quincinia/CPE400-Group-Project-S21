//
// CPE 400 Group Project Spring 2021
// Jacob Gayban, Ernesto Bernardo, Ritik Bhardwaj
//
// This file describes the Client (sender) class
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

    // to keep track of sequence numbers
    std::uint32_t sequence_num = 0;

public:
    Client() :
        tcp_socket(io_context)
    {
        // handle UDP stuff later
    }

    void tcp_connect() 
    {

    }

    void udp_connect() 
    {

    }

    std::string generate_metadata(std::fstream& file, char* file_name) 
    {

    }

    bool generate_packet(std::fstream& file, std::vector<char>& buf)
    {

    }

    
};
    
    
    