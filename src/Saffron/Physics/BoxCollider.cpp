#include "SaffronPCH.h"

#include "Saffron/Physics/BoxCollider.h"

namespace Se
{
auto BoxCollider::Create(const sf::Vector2f& position, const sf::Vector2f& dimensions) -> BoxCollider::Ptr
{
	Ptr ret = std::make_shared<BoxCollider>();
	ret->_dimensions = dimensions;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x + dimensions.x / 2.0f, position.y + dimensions.y / 2.0f);

	b2PolygonShape shape;
	shape.SetAsBox(ret->_dimensions.x / 2.0, ret->_dimensions.y / 2.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	ret->BuildBase(&bodyDef, &fixtureDef);

	return ret;
}

auto BoxCollider::GetRect() const -> sf::FloatRect
{
	const auto &pos = GetBody()->GetPosition();
	const sf::Vector2f topLeft = sf::Vector2f(pos.x, pos.y);
	return sf::FloatRect(topLeft, _dimensions);
}
}
