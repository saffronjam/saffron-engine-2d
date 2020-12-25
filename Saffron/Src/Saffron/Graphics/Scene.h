#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Saffron/Base.h"
#include "Saffron/Core/Camera.h"
#include "Saffron/Graphics/ControllableRenderTexture.h"

namespace Se
{
class Scene
{
private:
	class DrawCommand
	{
	public:
		DrawCommand(sf::Drawable &drawable, sf::RenderStates renderStates)
			: Drawable(drawable), RenderStates(renderStates)
		{
		}

	public:
		const sf::Drawable &Drawable;
		const sf::RenderStates RenderStates;
	};

public:
	Scene(ControllableRenderTexture *target, Camera *camera);

	void OnUpdate();

	void ActivateScreenSpaceDrawing() { _screenSpaceDrawing = true; }
	void DeactivateScreenSpaceDrawing() { _screenSpaceDrawing = false; }

	void Submit(const sf::Drawable &drawable, sf::RenderStates renderStates = sf::RenderStates());
	void Submit(const sf::Text &text, TextAlign align, sf::RenderStates renderStates = sf::RenderStates());
	void Submit(const sf::Vector2f &position, sf::Color color, float radius);
	void Submit(const sf::FloatRect &rect, sf::Color fillColor, bool outlined, sf::Color outlineColor);
	void Submit(const sf::Vector2f &first, const sf::Vector2f &second, sf::Color color);

	const sf::Vector2f &GetViewportSize() const { return _viewportSize; }
	void SetViewportSize(const sf::Vector2f &size) { _viewportSize = size; }

private:
	ArrayList<DrawCommand> _drawCommands;

	ControllableRenderTexture *_target;
	Camera *_camera;

	bool _screenSpaceDrawing = false;
	sf::Vector2f _viewportSize;
};
}
