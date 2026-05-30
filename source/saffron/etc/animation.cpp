#include "saffron_pch.h"

#include "saffron/etc/animation.h"

namespace saffron
{
Animation::Animation() :
	_texture(nullptr)
{
}

void Animation::addFrame(sf::IntRect rect)
{
	_frames.push_back(rect);
}

void Animation::setSpriteSheet(const sf::Texture& texture)
{
	_texture = &texture;
}

auto Animation::getSpriteSheet() const -> const sf::Texture*
{
	return _texture;
}

auto Animation::getSize() const -> std::size_t
{
	return _frames.size();
}

auto Animation::getFrame(std::size_t n) const -> const sf::IntRect&
{
	return _frames[n];
}
}
