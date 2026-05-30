#pragma once

#include "saffron/lighting/light.h"

namespace saffron
{
class PointLight : public Light
{
public:
	using Ptr = std::shared_ptr<PointLight>;

public:
	static auto Create(const sf::Vector2f& position, float radius, sf::Color color) -> PointLight::Ptr;
};
}
