#include "Light.h"

void Light::BuildBase(const sf::Vector2f &position, float radius, sf::Color color, std::pair<float, float> angle)
{
    m_position = position;
    m_radius = radius;
    m_color = color;
    m_angle = angle;
}