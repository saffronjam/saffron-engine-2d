#pragma once

#include <SFML/Graphics/VertexArray.hpp>

#include "saffron/lighting/light.h"

namespace saffron
{
class SpotLight : public Light
{
public:
	using Ptr = std::shared_ptr<SpotLight>;

public:
	static auto Create(const sf::Vector2f& position, const sf::Vector2f& direction, float theta, float radius,
	                   sf::Color color) -> SpotLight::Ptr;

	void SetDirection(const sf::Vector2f& newDirection, float theta);
};
}
