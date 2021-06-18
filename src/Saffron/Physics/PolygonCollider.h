#pragma once

#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>

#include "Saffron/Physics/PhysicsMgr.h"

namespace Se
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
