#include "TextureMgr.h"

std::map<std::string, sf::Texture> TextureMgr::m_textures;

TextureMgr::TextureMgr() noexcept
{
}

TextureMgr::~TextureMgr() noexcept
{
}

sf::Texture TextureMgr::Get(const std::string &filepath) noexcept
{
    for (auto &[path, tex] : m_textures)
    {
        if (path == filepath)
            return tex;
    }

    sf::Texture tex;
    tex.loadFromFile(filepath);
    m_textures.emplace(std::make_pair(filepath, tex));

    return tex;
}