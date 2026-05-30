#include "saffron_pch.h"

#include "saffron/lighting/point_light.h"

namespace saffron
{
auto PointLight::Create(const sf::Vector2f& position, float radius, sf::Color color) -> PointLight::Ptr
{
	Ptr ret = std::make_shared<PointLight>();
	ret->BuildBase(position, radius, color, VecUtils::Null<>(), 0.0f);
	return ret;
}
}
