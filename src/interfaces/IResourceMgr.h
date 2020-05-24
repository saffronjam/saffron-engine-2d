#pragma once

#include <map>

template <class T>
class IResourceMgr
{
public:
    IResourceMgr() = default;
    // Returns pointer resource from cache, if not existing, call Load();
    virtual T *Get(const std::string &filepath) noexcept
    {
        if (m_resources.find(filepath) == m_resources.end())
        {
            Load(filepath);
        }
        return &m_resources[filepath];
    }
    // Returns copy of resource from cache, if not existing, call Load();
    virtual const T &GetCopy(const std::string &filepath) noexcept
    {
        if (m_resources.find(filepath) == m_resources.end())
        {
            Load(filepath);
        }
        return m_resources[filepath];
    }
    // Load resource into memory
    virtual void Load(const std::string &filepath) noexcept = 0;

protected:
    std::map<std::string, T> m_resources;
};