#pragma once

#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>

#include "saffron/base.h"
#include "saffron/physics/physics_mgr.h"

namespace saffron
{
class CircleCollider : public Collider
{
public:
	using Ptr = std::shared_ptr<CircleCollider>;

public:
	static auto Create(const sf::Vector2f& position, float radius) -> Ptr;

	auto GetRadius() const -> float { return _radius; }

private:
	float _radius;
};
}
