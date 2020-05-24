#pragma once

#include <map>

template <class T>
class ResourceMgr
{
public:
    ResourceMgr() = default;
    // Get resource from cache, if not existing, call Load();
    virtual T *Get(const std::string &filepath) noexcept
    {
        if (m_resources.find(filepath) == m_resources.end())
        {
            Load(filepath);
        }
        return &m_resources[filepath];
    }
    // Load resource into memory
    virtual void Load(const std::string &filepath) noexcept
    {
        T resource;
        resource.loadFromFile(filepath);
        m_resources.emplace(std::make_pair(filepath, resource));
    }

protected:
    std::map<std::string, T> m_resources;
};