#pragma once

#include "Clock.h"

class IConnInfo
{
public:
    IConnInfo(sf::Uint64 uid);
    bool operator<(const IConnInfo &rhs) const
    {
        return m_uid > rhs.m_uid;
    }
    sf::Uint64 GetUID() const noexcept { return m_uid; }
    virtual void UpdateTimeout() { m_timeout += Clock::Delta(); }
    virtual bool IsTimedOut() const noexcept { m_currTimeoutTimer >= m_timeout; }
    virtual void SetTimeout(sf::Time seconds) noexcept { m_timeout = seconds; }
    virtual void Reset();

private:
    const sf::Uint64 m_uid;

    sf::Time m_currTimeoutTimer;
    sf::Time m_timeout;
};