//
// CPE 400 Group Project Spring 2021
// Jacob Gayban, Ernesto Bernardo, Ritik Bhardwaj
//
// This is the program for the RECEIVER of the file.
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

int main(int argc, char* argv[])
{
    try
    {
        // all asio programs need an io_context
        boost::asio::io_context io_context;

        // listener for connections
        // this application will use port 1234
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 1234));

        // socket represents the connection taking place
        tcp::socket tcp_socket(io_context);

        // UDP server stuff
        udp::socket udp_socket(io_context, udp::endpoint(udp::v4(), 1235));
        udp::endpoint remote_endpoint;

        // wait for connection
        acceptor.accept(tcp_socket);

        // receive metadata from client
        boost::asio::streambuf receive_buffer;
        boost::system::error_code error;
        boost::asio::read(tcp_socket, receive_buffer, boost::asio::transfer_all(), error);
        if (error && error != boost::asio::error::eof)
        {
            std::cout << "receive failed: " << error.message() << std::endl;
        }
        else
        {
            const char *data = boost::asio::buffer_cast<const char *>(receive_buffer.data());
            std::cout << data << std::endl;
        }

        // initialize new file to be made

        // return ACK for metadata (may not be needed)

        /*
        do {
            // retrieve data
            // confirm sequence number
            // write to file if the number is good
            // return ACK
        } while(this packet isn't the last packet)
        */

       // close file and end connections
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}