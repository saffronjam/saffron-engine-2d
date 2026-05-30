#include "saffron_pch.h"

#include "saffron/graphics/controllable_render_texture.h"

namespace saffron
{
ControllableRenderTexture::ControllableRenderTexture(int width, int height, sf::ContextSettings contextSettings)
{
	const auto result = TryCreate(width, height, contextSettings);
	if (!result)
	{
		Log::CoreError(result.error().message);
	}
}

auto ControllableRenderTexture::TryCreate(int width, int height, sf::ContextSettings contextSettings) -> Status
{
	if (!_renderTexture.create(width, height, contextSettings))
	{
		return std::unexpected(Error{ErrorCode::Graphics,
		                            "Failed to create render texture " + std::to_string(width) + "x" +
		                                std::to_string(height)});
	}

	return {};
}

auto ControllableRenderTexture::RenderTexture() -> sf::RenderTexture& { return _renderTexture; }

auto ControllableRenderTexture::RenderTexture() const -> const sf::RenderTexture& { return _renderTexture; }

auto ControllableRenderTexture::Texture() const -> const sf::Texture& { return _renderTexture.getTexture(); }

auto ControllableRenderTexture::IsEnabled() const -> bool { return _enabled; }

void ControllableRenderTexture::Enable()
{
	_enabled = true;
}

void ControllableRenderTexture::Disable()
{
	_enabled = false;
}

auto ControllableRenderTexture::ClearColor() const -> sf::Color { return _clearColor; }

void ControllableRenderTexture::SetClearColor(sf::Color color)
{
	_clearColor = color;
}
}
