#include "IConnInfo.h"

IConnInfo::IConnInfo(sf::Uint64 uid)
    : m_uid(uid)
{
}

void IConnInfo::Reset()
{
    m_currTimeoutTimer = sf::Time();
}
