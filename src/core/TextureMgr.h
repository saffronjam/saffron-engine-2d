#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "IResourceMgr.h"

class TextureMgr : public ResourceMgr<sf::Texture>
{
public:
    TextureMgr();
    TextureMgr(const TextureMgr &) = delete;
    const TextureMgr &operator()(const TextureMgr &) = delete;
};