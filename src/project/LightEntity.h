#pragma once

#include <SFML/System/Vector2.hpp>

#include "PointLight.h"

struct LightEntity
{
    sf::Vector2f direction;
    float speed;
    SpotLight::Ptr light;
};
