#pragma once

#include "ParsedPacket.h"
#include "Connection.h"

#define HandlePacket(type, packet) Handle##type##Packet(packet)
#define IConnectionCast(connP, protocol) ((protocol) == Protocol::TCP ? I)
#define IConnectionCastTCP(connP) dynamic_cast<const Connection<sf::TcpSocket> *>(connP)
#define IConnectionCastUDP(connP) dynamic_cast<const Connection<sf::UdpSocket> *>(connP)

class IPacketHandler
{
    friend class PacketMgr;

protected:
    virtual void HandlePacket(Text, const ParsedPacket &packet) {}
    virtual void HandlePacket(AreYouAlive, const ParsedPacket &packet) {}
    virtual void HandlePacket(IAmAlive, const ParsedPacket &packet) {}
};