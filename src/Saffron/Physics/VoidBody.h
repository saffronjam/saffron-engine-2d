#pragma once

#include <Box2D/Box2D.h>
#include <SFML/System/Vector2.hpp>

#include "Saffron/Physics/PhysicsMgr.h"

namespace Se
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
