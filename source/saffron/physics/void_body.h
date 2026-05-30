#pragma once

#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>

#include "saffron/physics/physics_mgr.h"

namespace saffron
{
class VoidBody : public Collider
{
public:
	using Ptr = std::shared_ptr<VoidBody>;

public:
	static auto Create(const sf::Vector2f& position) -> Ptr;

private:
};
}
