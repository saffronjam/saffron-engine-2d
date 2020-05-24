#pragma once

#include <SFML/Graphics/Font.hpp>

#include "IResourceMgr.h"

class FontMgr : public ResourceMgr<sf::Font>
{
public:
    FontMgr();
    FontMgr(const FontMgr &) = delete;
    const FontMgr &operator()(const FontMgr &) = delete;
};