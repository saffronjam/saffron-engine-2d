#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "Saffron/Base.h"

namespace Se
{
class ControllableRenderTexture
{
public:
	ControllableRenderTexture(int width, int height, sf::ContextSettings contextSettings = sf::ContextSettings());
	operator sf::RenderTexture&();
	operator const sf::RenderTexture&() const;
	operator const sf::Texture&() const;

	auto RenderTexture() -> sf::RenderTexture&;
	auto RenderTexture() const -> const sf::RenderTexture&;

	auto IsEnabled() const -> bool;
	void Enable();
	void Disable();

	auto ClearColor() const -> sf::Color;
	void SetClearColor(sf::Color color);

private:
	sf::RenderTexture _renderTexture;
	bool _enabled = true;
	sf::Color _clearColor = sf::Color::Black;
};
}
