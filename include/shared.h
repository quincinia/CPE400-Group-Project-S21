//
// CPE 400 Group Project Spring 2021
// Jacob Gayban, Ernesto Bernardo, Ritik Bhardwaj
//
// This file contains structures used by both the server and client
//
#ifndef SHARED
#define SHARED

#include <cstdint>

#define TCP_PORT    1234
#define UDP_PORT    1235

#define PACKET_SIZE 1024

union sequence_num_translator
{
    std::uint32_t sequence_num;
    char translate[4];
};

#endif