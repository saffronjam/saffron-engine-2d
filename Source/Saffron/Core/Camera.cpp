#include "SaffronPCH.h"

#include "Saffron/Core/App.h"
#include "Saffron/Core/Camera.h"
#include "Saffron/Core/Global.h"
#include "Saffron/Gui/Gui.h"
#include "Saffron/Input/Keyboard.h"
#include "Saffron/Input/Mouse.h"
#include "Saffron/Lighting/LightningMgr.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Saffron/Base.h"
#include "Saffron/Core/Window.h"
#include "Saffron/Libraries/GenUtils.h"

namespace Se
{
Camera::Camera() :
	_transform(sf::Transform::Identity),
	_positionTransform(sf::Transform::Identity),
	_rotationTransform(sf::Transform::Identity),
	_zoomTransform(sf::Transform::Identity),
	_position(0.0f, 0.0f),
	_rotation(0.0f),
	_rps(0.2f),
	_zoom(1.0f, 1.0f)
{
}

void Camera::OnUpdate()
{
	if (!_enabled) return;

	const auto dt = Global::Clock::FrameTime();

	if (_follow.has_value())
	{
		SetCenter(*_follow.value());
	}
	else
	{
		if (Mouse::IsDown(sf::Mouse::Button::Left) && Mouse::IsDown(sf::Mouse::Button::Right))
		{
			sf::Vector2f delta = Mouse::Swipe();
			if (VecUtils::LengthSq(delta) > 0.0f)
			{
				delta = _rotationTransform.getInverse().transformPoint(delta);
				delta = _zoomTransform.getInverse().transformPoint(delta);
				delta *= -1.0f;
				ApplyMovement(delta);
			}
		}
	}

	ApplyZoom((Mouse::VerticalScroll() / 100.0f) + 1.0f);

	float angle = 0.0f;

	if (Keyboard::IsDown(sf::Keyboard::Q))
	{
		angle += _rps * 360.0f * dt.asSeconds();
	}
	if (Keyboard::IsDown(sf::Keyboard::E))
	{
		angle -= _rps * 360.0f * dt.asSeconds();
	}
	ApplyRotation(angle);

	if (Keyboard::IsPressed(sf::Keyboard::R))
	{
		ResetTransformation();
	}
}

void Camera::OnGuiRender()
{
	if (ImGui::Begin("Camera"))
	{
		Gui::BeginPropertyGrid();
		Gui::Property("Position", _position, -10000.0f, 10000.0f, 10.0f, GuiPropertyFlag_Drag);
		if (Gui::Property("Zoom", _zoom.x, 0.01f, 10.0f, 0.01f, GuiPropertyFlag_Slider))
		{
			_zoom.y = _zoom.x;
		}
		Gui::Property("Rotation", _rotation, -180.0f, 180.0f, 1.0f, GuiPropertyFlag_Slider);
		Gui::Property("Rotation Speed", _rps, 0.0f, 10.0f, 0.1f, GuiPropertyFlag_Slider);

		if (_follow.has_value())
		{
			auto follow = *_follow.value();
			Gui::Property("Follow", follow, GuiPropertyFlag_Drag);
		}
		else
		{
			Gui::Property("Follow", "Not set");
		}

		Gui::EndPropertyGrid();
	}
	ImGui::End();
}

auto Camera::Enabled() const -> bool
{
	return _enabled;
}

void Camera::Enable()
{
	_enabled = true;
}

void Camera::Disable()
{
	_enabled = false;
}

void Camera::ApplyMovement(const sf::Vector2f& offset)
{
	SetCenter(_position + offset);
}

void Camera::ApplyZoom(float factor)
{
	_zoom *= factor;
	_zoomTransform.scale(factor, factor);
	UpdateTransform();
}

void Camera::ApplyRotation(float angle)
{
	SetRotation(_rotation + angle);
}

void Camera::SetCenter(const sf::Vector2f& center)
{
	_position = center;
	_positionTransform = sf::Transform().translate(_position);
	UpdateTransform();
}

void Camera::SetZoom(float zoom)
{
	if (zoom != 0.0f)
	{
		LightningMgr::ChangeResolution(zoom / _zoom.x);
		_zoom = sf::Vector2f(zoom, zoom);
		_zoomTransform = sf::Transform().scale(_zoom, _zoom);
		UpdateTransform();
	}
}

void Camera::SetRotation(float angle)
{
	_rotation = angle;
	_rotationTransform = sf::Transform().rotate(_rotation);
	UpdateTransform();
}

void Camera::Follow(const sf::Vector2f* follow)
{
	_follow = follow;
}

void Camera::Unfollow()
{
	_follow.reset();
}

auto Camera::ScreenToWorld(const sf::Vector2f& point) const -> sf::Vector2f
{
	return _transform.getInverse().transformPoint(point);
}

auto Camera::ScreenToWorld(const sf::FloatRect& rect) const -> sf::FloatRect
{
	return _transform.getInverse().transformRect(rect);
}

auto Camera::WorldToScreen(const sf::Vector2f& point) const -> sf::Vector2f
{
	return _transform.transformPoint(point);
}

auto Camera::WorldToScreen(const sf::FloatRect& rect) const -> sf::FloatRect
{
	return _transform.transformRect(rect);
}

auto Camera::Transform() const -> const sf::Transform&
{
	return _transform;
}

auto Camera::Zoom() const -> float
{
	return _zoom.x;
}

auto Camera::ViewportSize() const -> sf::Vector2f
{
	return _viewportSize;
}

void Camera::SetViewportSize(const sf::Vector2f& viewportSize)
{
	_viewportSize = viewportSize;
}

void Camera::SetRotationSpeed(float rps)
{
	_rps = rps;
}

void Camera::SetTransform(const sf::Transform& transform)
{
	_transform = transform;
}

auto Camera::Viewport() const -> Pair<sf::Vector2f, sf::Vector2f>
{
	const sf::FloatRect screenRect = {{0.0f, 0.0f}, _viewportSize};
	const auto TL = sf::Vector2f(screenRect.left, screenRect.top);
	const auto BR = sf::Vector2f(screenRect.left + screenRect.width, screenRect.top + screenRect.height);

	return std::make_pair(_transform.getInverse().transformPoint(TL), _transform.getInverse().transformPoint(BR));
}

auto Camera::Offset() const -> sf::Vector2f
{
	return _viewportSize / 2.0f;
}

void Camera::UpdateTransform()
{
	_transform = sf::Transform::Identity;
	_transform.translate(Offset());
	_transform.scale(_zoom);
	_transform.rotate(_rotation);
	_transform.translate(-_position);
}

void Camera::CapZoomLevel() const
{
	GenUtils::Constrain(_zoom.x, 0.9f, 3.0f);
	GenUtils::Constrain(_zoom.y, 0.9f, 3.0f);
}

void Camera::ResetTransformation()
{
	SetCenter(sf::Vector2f(0.0f, 0.0f));
	SetRotation(0.0f);
	SetZoom(1.0f);

	_positionTransform = sf::Transform::Identity;
	_rotationTransform = sf::Transform::Identity;
	_zoomTransform = sf::Transform::Identity;

	Reset.Invoke();
}
}
