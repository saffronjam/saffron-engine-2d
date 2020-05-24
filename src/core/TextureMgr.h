#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "IResourceMgr.h"

class TextureMgr : public IResourceMgr<sf::Texture>
{
public:
    TextureMgr() = default;
    TextureMgr(const TextureMgr &) = delete;
    const TextureMgr &operator()(const TextureMgr &) = delete;

    virtual void Load(const std::string &filepath) noexcept override
    {
        sf::Texture resource;
        resource.loadFromFile(filepath);
        m_resources.emplace(std::make_pair(filepath, resource));
    }
};