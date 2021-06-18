#pragma once

#include "Saffron/Base.h"
#include "Saffron/Graphics/ControllableRenderTexture.h"

namespace Se
{
class ViewportPane
{
public:
	explicit ViewportPane(String windowTitle, const ControllableRenderTexture& target);

	void OnGuiRender(bool* open = nullptr, UUID uuid = 0);

	auto InViewport(sf::Vector2f positionNDC) const -> bool;

	auto MousePosition(bool normalized = false) const -> sf::Vector2f;
	auto ViewportSize() const -> sf::Vector2f;

	auto DockID() const -> Uint32;

	auto TopLeft() const -> const sf::Vector2f&;
	auto BottomRight() const -> const sf::Vector2f&;

	auto Hovered() const -> bool;
	auto Focused() const -> bool;

	void SetTarget(const ControllableRenderTexture& target);

public:
	EventSubscriberList<void> Rendered;
	EventSubscriberList<const sf::Vector2f&> Resized;

private:
	String _windowTitle;
	const ControllableRenderTexture* _target;
	sf::Texture _fallbackTexture;
	Uint32 _dockID = 0;

	sf::Vector2f _topLeft;
	sf::Vector2f _bottomRight;
	bool _hovered;
	bool _focused;
};
}
