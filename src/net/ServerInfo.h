#pragma once

#include "IConnInfo.h"

class ServerInfo : public IConnInfo
{
public:
    ServerInfo(sf::Uint64 uid)
        : IConnInfo(uid)
    {
    }
};