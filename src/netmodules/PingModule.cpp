#include "PingModule.h"

#include "INetMgr.h"

PingModule::PingModule(INetMgr *ioHelper)
    : INetModule(ioHelper)
{
}

void PingModule::HandlePacket(AreYouAlive, const ParsedPacket &packet)
{
    log_info("Client asking if I'm alive, of course I am...");
    m_ioHelper->SendEmpty<UDP>(IAmAlive, packet.connection);
}

void PingModule::HandlePacket(IAmAlive, const ParsedPacket &packet)
{
    log_info("Client said they are alive...");
}