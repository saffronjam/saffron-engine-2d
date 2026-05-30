#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "saffron/base.h"

namespace saffron
{
class ControllableRenderTexture
{
public:
	ControllableRenderTexture(int width, int height, sf::ContextSettings contextSettings);

	auto TryCreate(int width, int height, sf::ContextSettings contextSettings) -> Status;

	auto RenderTexture() -> sf::RenderTexture&;
	auto RenderTexture() const -> const sf::RenderTexture&;
	auto Texture() const -> const sf::Texture&;

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
