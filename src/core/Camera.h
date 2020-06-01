#pragma once

#include <optional>

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "Window.h"
#include "Lib.h"

class Camera
{
public:
    Camera(const sf::Vector2f &position = sf::Vector2f(0.0f, 0.0f));
    ~Camera();
    Camera(const Camera &camera);
    const Camera &operator()(const Camera &camera);

    // Updates cameras position relative to have follow-target in the middle
    // If no follow-target has been set this function does nothing
    void Update();
    void Draw(const sf::Drawable &drawable, sf::RenderStates renderStates = sf::RenderStates::Default) const noexcept;

    void Move(const sf::Vector2f &offset) noexcept;
    void Zoom(float factor) noexcept;
    void Rotate(float angle) noexcept;

    void SetViewport(const sf::FloatRect &viewport) noexcept;
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

private:
    sf::View m_view;
    sf::FloatRect m_viewport;
    std::optional<sf::Vector2f *> m_follow;
    float m_rotation;
    float m_zoom;
};