#pragma once

#include <map>

#include <SFML/Graphics/Texture.hpp>

#include "Window.h"

class TextureMgr
{
public:
    TextureMgr() noexcept;
    ~TextureMgr() noexcept;
    TextureMgr(const TextureMgr &) = delete;
    const TextureMgr &operator()(const TextureMgr &) = delete;

    static sf::Texture Get(const std::string &filepath) noexcept;

private:
    static std::map<std::string, sf::Texture> m_textures;
};