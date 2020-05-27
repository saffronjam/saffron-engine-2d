#pragma once

#include <string>

class ClientInfo
{
public:
    bool operator<(const ClientInfo &rhs) const
    {
        return true;
    }
    std::string GetName() const noexcept { return m_name; }
    void SetName(const std::string &name) noexcept { m_name = name; }

private:
    std::string m_name;
};