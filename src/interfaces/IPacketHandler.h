#pragma once

#include "ParsedPacket.h"

#define HandlePacket(type, packet) Handle##type##Packet(packet)

class IPacketHandler
{
    friend class PacketMgr;

protected:
    virtual void HandlePacket(Text, ParsedPacket &packet) = 0;
    virtual void HandlePacket(AreYouAlive, ParsedPacket &packet) = 0;
    virtual void HandlePacket(IAmAlive, ParsedPacket &packet) = 0;
};