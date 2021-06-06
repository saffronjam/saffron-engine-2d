#include "SaffronPCH.h"

#include "Saffron/Graphics/ControllableRenderTexture.h"

namespace Se
{
ControllableRenderTexture::ControllableRenderTexture(int width, int height, sf::ContextSettings contextSettings)
{
	_renderTexture.create(width, height, contextSettings);
}

ControllableRenderTexture::operator sf::RenderTexture&()
{
	return _renderTexture;
}

ControllableRenderTexture::operator const sf::RenderTexture&() const
{
	return _renderTexture;
}

ControllableRenderTexture::operator const sf::Texture&() const
{
	return _renderTexture.getTexture();
}

auto ControllableRenderTexture::GetRenderTexture() -> sf::RenderTexture& { return _renderTexture; }

auto ControllableRenderTexture::GetRenderTexture() const -> const sf::RenderTexture& { return _renderTexture; }

auto ControllableRenderTexture::IsEnabled() const -> bool { return _enabled; }

void ControllableRenderTexture::Enable()
{
	_enabled = true;
}

void ControllableRenderTexture::Disable()
{
	_enabled = false;
}

auto ControllableRenderTexture::GetClearColor() const -> sf::Color { return _clearColor; }

void ControllableRenderTexture::SetClearColor(sf::Color color)
{
	_clearColor = color;
}
}
