#include "saffron_pch.h"

#include "saffron/lighting/light.h"

namespace saffron
{
void Light::BuildBase(const sf::Vector2f& position, float radius, sf::Color color, const sf::Vector2f& direction,
                      float theta)
{
	_position = position;
	_radius = radius;
	_color = color;
	const auto bounds = GetBoundingBox();
	if (!_staticOccluderMap.create(bounds.width, bounds.height))
	{
		Log::CoreError("Failed to create static occluder map {0}x{1}", bounds.width, bounds.height);
		return;
	}
	_staticOccluderMap.clear(sf::Color(0, 0, 0, 0));
}

void Light::AddStaticOccluder(const sf::Drawable& drawable, const sf::RenderStates& renderStates)
{
	_staticOccluderMap.draw(drawable, renderStates);
	_staticOccluderMap.display();
}
}
