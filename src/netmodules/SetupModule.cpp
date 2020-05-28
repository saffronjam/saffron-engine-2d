#include "SetupModule.h"

#include "INetMgr.h"

SetupModule::SetupModule(INetMgr *ioHelper)
    : INetModule(ioHelper)
{
}

void SetupModule::HandlePacket(UID, const ParsedPacket &packet)
{
    log_info("Received ID!");
    long uid = *reinterpret_cast<long *>(packet.data.get());
    m_ioHelper->SetUID(uid);
}

void SetupModule::HandlePacket(UDPSync, const ParsedPacket &packet)
{
}
