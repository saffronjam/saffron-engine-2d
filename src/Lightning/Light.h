#pragma once

#include <utility>
#include <memory>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class Light
{
public:
    using Ptr = std::shared_ptr<Light>;

protected:
    void BuildBase(const sf::Vector2f &position, float radius, sf::Color color = sf::Color::White, std::pair<float, float> angle = std::make_pair(0.0f, 0.0f));

public:
    const sf::Vector2f &GetPosition() const noexcept { return m_position; }
    float GetRadius() const noexcept { return m_radius; }
    const sf::Color &GetColor() const noexcept { return m_color; }

    void SetPosition(const sf::Vector2f &position) noexcept { m_position = position; }
    void SetRadius(float radius) noexcept { m_radius = radius; }
    void SetColor(const sf::Color &color) noexcept { m_color = color; }

protected:
    sf::Vector2f m_position;
    float m_radius;
    sf::Color m_color;
    std::pair<float, float> m_angle;
};