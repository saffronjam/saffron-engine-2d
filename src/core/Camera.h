#pragma once

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

    void Draw(const sf::Drawable &drawable, sf::RenderStates renderStates = sf::RenderStates::Default) const noexcept;

    void Move(const sf::Vector2f &offset) noexcept;
    void Zoom(float factor) noexcept;
    void Rotate(float angle) noexcept;

    void SetViewport(const sf::FloatRect &viewport) noexcept;
    void SetCenter(const sf::Vector2f &center) noexcept;
    void SetZoom(float factor) noexcept;
    void SetRotation(float angle) noexcept;
    void SetFollow(sf::Vector2f *follow) noexcept;

    ///Translate a point to view space
    ///@param point: point to be translated to view space. Excepted to be in screen space
    sf::Vector2f ScreenToView(const sf::Vector2f &point) const noexcept;
    ///Translate a point to world space
    ///@param point: point to be translated to world space. Excepted to be in screen space
    sf::Vector2f ScreenToWorld(const sf::Vector2f &point) const noexcept;

private:
    sf::View m_view;
    sf::Vector2f m_position;
    sf::Vector2f *m_follow;
    float m_zoom;
};