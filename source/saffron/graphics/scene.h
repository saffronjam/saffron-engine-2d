#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "saffron/base.h"
#include "saffron/core/camera.h"
#include "saffron/core/enums.h"
#include "saffron/graphics/controllable_render_texture.h"
#include "saffron/gui/viewport_pane.h"

namespace saffron
{
class Scene
{
public:
	Scene(std::string name, ControllableRenderTexture* target, Camera* camera);

	void OnUpdate() const;
	void OnGuiRender();
	void OnRenderTargetResize(const sf::Vector2f& size) const;

	void ActivateScreenSpaceDrawing();
	void DeactivateScreenSpaceDrawing();

	void Submit(const sf::Drawable& drawable, sf::RenderStates renderStates) const;
	void Submit(const sf::Text& text, TextAlign align, sf::RenderStates renderStates);
	void Submit(const sf::Vector2f& position, sf::Color color, float radius);
	void Submit(const sf::FloatRect& rect, sf::Color fillColor, bool outlined, sf::Color outlineColor);
	void Submit(const sf::Vector2f& first, const sf::Vector2f& second, sf::Color color);

	auto Camera() -> class Camera&;
	auto Camera() const -> const class Camera&;

	auto ViewportPane() -> class ViewportPane&;
	auto ViewportPane() const -> const class ViewportPane&;

private:
	ControllableRenderTexture* _target;
	class Camera* _camera;
	class ViewportPane _viewportPane;

	bool _screenSpaceDrawing = false;
};
}
