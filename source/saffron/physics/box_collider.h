#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "saffron/base.h"
#include "saffron/physics/physics_mgr.h"

namespace saffron
{
class BoxCollider : public Collider
{
public:
	using Ptr = std::shared_ptr<BoxCollider>;

public:
	static auto Create(const sf::Vector2f& position, const sf::Vector2f& dimensions) -> Ptr;

	auto GetDimensions() const -> const sf::Vector2f& { return _dimensions; }

	auto GetRect() const -> sf::FloatRect;

private:
	sf::Vector2f _dimensions;
};
}
