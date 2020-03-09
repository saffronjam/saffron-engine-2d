#pragma once

#include <string>

#define HEADER_SIZE 10
#define QUERY_SIZE 15

class PacketT
{  
public:
    PacketT(std::string query, void* data, int dataSize)
        :
        query(query),
        data(data),
        dataSize(dataSize)
    {
    }
    std::string query;
    void* data;
    int dataSize;
};