#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Saffron/Base.h"
#include "Saffron/Core/Camera.h"
#include "Saffron/Graphics/ControllableRenderTexture.h"
#include "Saffron/Gui/ViewportPane.h"

namespace Se
{
class Scene
{
public:
	Scene(String name, ControllableRenderTexture* target, Camera* camera);

	void OnUpdate() const;
	void OnGuiRender();
	void OnRenderTargetResize(const sf::Vector2f& size) const;

	void ActivateScreenSpaceDrawing() { _screenSpaceDrawing = true; }

	void DeactivateScreenSpaceDrawing() { _screenSpaceDrawing = false; }

	void Submit(const sf::Drawable& drawable, sf::RenderStates renderStates = sf::RenderStates()) const;
	void Submit(const sf::Text& text, TextAlign align, sf::RenderStates renderStates = sf::RenderStates());
	void Submit(const sf::Vector2f& position, sf::Color color, float radius);
	void Submit(const sf::FloatRect& rect, sf::Color fillColor, bool outlined, sf::Color outlineColor);
	void Submit(const sf::Vector2f& first, const sf::Vector2f& second, sf::Color color);

	auto GetCamera() -> Camera& { return *_camera; }

	auto GetCamera() const -> const Camera& { return *_camera; }

	auto GetViewportPane() -> ViewportPane& { return _viewportPane; }

	auto GetViewportPane() const -> const ViewportPane& { return _viewportPane; }

private:
	ControllableRenderTexture* _target;
	Camera* _camera;
	ViewportPane _viewportPane;

	bool _screenSpaceDrawing = false;
};
}
