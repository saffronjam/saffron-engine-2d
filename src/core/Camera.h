#pragma once

#include <optional>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "Window.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Clock.h"
#include "Lib.h"
#include "VectorLib.h"

class Camera
{
public:
    Camera();
    ~Camera();

    // Updates cameras position relative to have follow-target in the middle
    // If no follow-target has been set this function does nothing
    void Update();
    void Draw(const sf::Drawable &drawable, sf::RenderStates renderStates = sf::RenderStates::Default) const noexcept;

    void Move(const sf::Vector2f &offset) noexcept;
    void Zoom(float factor) noexcept;
    void Rotate(float angle) noexcept;

    void SetCenter(const sf::Vector2f &center) noexcept;
    void SetZoom(float factor) noexcept;
    void SetRotation(float angle) noexcept;

    void Follow(sf::Vector2f *follow) noexcept { m_follow = follow; }
    void Unfollow() noexcept { m_follow.reset(); }

    ///Translate a point to world space
    ///@param point: point to be translated from screen to world space.
    sf::Vector2f ScreenToWorld(const sf::Vector2f &point) const noexcept;
    ///Translate a point to world space
    ///@param point: point to be translated from world to screen space.
    sf::Vector2f WorldToScreen(const sf::Vector2f &point) const noexcept;

    void SetRotationSpeed(float rps) noexcept { m_rps = rps; }

private:
    void UpdateTransform() noexcept;
    void CapZoomLevel() noexcept;
    void ResetTransformation() noexcept;

private:
    sf::Transform m_transform;
    std::optional<sf::Vector2f *> m_follow;

    sf::Vector2f m_position;
    sf::Transform m_positionTransform;

    float m_rotation;
    sf::Transform m_rotationTransform;
    float m_rps; // Rotations per second

    sf::Vector2f m_zoom;
    sf::Transform m_zoomTransform;

    bool m_engaged;
    sf::Vector2f m_lastPos;
};