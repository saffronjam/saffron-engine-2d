#pragma once

#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>

#include "saffron/physics/physics_mgr.h"

namespace saffron
{
class PolygonCollider : public Collider
{
public:
	using Ptr = std::shared_ptr<PolygonCollider>;

public:
	static auto Create(const sf::Vector2f& position, std::vector<sf::Vector2f> points) -> Ptr;

private:
};
}
