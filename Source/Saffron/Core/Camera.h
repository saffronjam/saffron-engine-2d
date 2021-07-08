#pragma once

#include <optional>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Saffron/Base.h"

namespace Se
{
class Camera
{
public:
	Camera();

	void OnUpdate();
	void OnGuiRender();

	auto Enabled() const -> bool;
	void Enable();
	void Disable();

	void ApplyMovement(const sf::Vector2f& offset);
	void ApplyZoom(float factor);
	void ApplyRotation(float angle);

	void SetCenter(const sf::Vector2f& center);
	void SetZoom(float zoom);
	void SetRotation(float angle);

	void Follow(const sf::Vector2f* follow);
	void Unfollow();

	///Translate a point to world space
	///@param point: point to be translated from screen to world space.
	auto ScreenToWorld(const sf::Vector2f& point) const -> sf::Vector2f;
	///Translate a rect to world space
	///@param rect: rect to be translated from screen to world space.
	auto ScreenToWorld(const sf::FloatRect& rect) const -> sf::FloatRect;
	///Translate a point to world space
	///@param point: point to be translated from screen to screen space.
	auto WorldToScreen(const sf::Vector2f& point) const -> sf::Vector2f;
	///Translate a rect to world space
	///@param rect: rect to be translated from world to screen space.
	auto WorldToScreen(const sf::FloatRect& rect) const -> sf::FloatRect;

	auto Transform() const -> const sf::Transform&;
	auto Viewport() const -> Pair<sf::Vector2f, sf::Vector2f>;
	auto Offset() const -> sf::Vector2f;
	auto Zoom() const -> float;

	auto ViewportSize() const -> sf::Vector2f;
	void SetViewportSize(const sf::Vector2f& viewportSize);
	void SetRotationSpeed(float rps);

	void SetTransform(const sf::Transform &transform);

public:
	EventSubscriberList<void> Reset;

private:
	void UpdateTransform();
	void CapZoomLevel() const;
	void ResetTransformation();

private:
	bool _enabled = true;

	sf::Transform _transform;
	sf::Transform _positionTransform;
	sf::Transform _rotationTransform;
	sf::Transform _zoomTransform;

	sf::Vector2f _position;
	float _rotation;
	float _rps; // Rotations per second
	sf::Vector2f _zoom;
	sf::Vector2f _viewportSize;

	Optional<const sf::Vector2f*> _follow;
};
}
