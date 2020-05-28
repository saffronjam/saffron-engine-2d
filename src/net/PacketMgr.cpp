#include "PacketMgr.h"

#define SwitchHandle(type, packet)             \
    case type:                                 \
        handler->Handle##type##Packet(packet); \
        break;

std::set<std::pair<PacketBuffer *, std::mutex *>> PacketMgr::m_packetBuffers;
std::set<IPacketHandler *> PacketMgr::m_handlers;

void PacketMgr::AddPacketBuffer(PacketBuffer *buffer, std::mutex *mutex)
{
    m_packetBuffers.emplace(std::make_pair(buffer, mutex));
}

void PacketMgr::AddHandler(IPacketHandler *handler)
{
    m_handlers.emplace(handler);
}

void PacketMgr::RemovePacketBuffer(PacketBuffer *buffer)
{
    for (auto &pair : m_packetBuffers)
    {
        if (pair.first == buffer)
        {
            m_packetBuffers.erase(pair);
            return;
        }
    }
}

void PacketMgr::RemoveHandler(IPacketHandler *handler)
{
    m_handlers.erase(handler);
}

void PacketMgr::HandleAllPackets()
{
    for (auto &[packetBuffer, mutex] : m_packetBuffers)
    {
        mutex->lock();
        for (auto &packet : *packetBuffer)
        {
            for (auto &handler : m_handlers)
            {
                log_info("Do i fail here?");
                switch (packet.type)
                {
                    SwitchHandle(Text, packet);
                    SwitchHandle(AreYouAlive, packet);
                    SwitchHandle(IAmAlive, packet);
                }
                log_info("Nope!");
            }
        }
        packetBuffer->clear();
        mutex->unlock();
    }
}