#pragma once

#include <vector>

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace Se
{
class Animation
{
public:
	Animation();

	void addFrame(sf::IntRect rect);
	void setSpriteSheet(const sf::Texture& texture);
	auto getSpriteSheet() const -> const sf::Texture*;
	auto getSize() const -> std::size_t;
	auto getFrame(std::size_t n) const -> const sf::IntRect&;

private:
	std::vector<sf::IntRect> _frames;
	const sf::Texture* _texture;
};
}
