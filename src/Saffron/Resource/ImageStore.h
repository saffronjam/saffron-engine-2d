#pragma once

#include <SFML/Graphics/Image.hpp>

#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class ImageStore : public ResourceStore<sf::Image>
{
public:
	static auto Get(const Path& Path, bool copy = false) -> Shared<sf::Image>
	{
		return Instance().Fetch(Path, copy);
	}

private:
	auto Copy(const Shared<sf::Image>& value) -> Shared<sf::Image> override
	{
		auto resource = CreateShared<sf::Image>();
		resource->copy(*value, 0, 0);
		return resource;
	}

	auto Location() -> Path override
	{
		return "res/Textures/";
	}

private:
	auto Load(Path Path) -> Shared<sf::Image> override
	{
		auto resource = CreateShared<sf::Image>();
		const auto result = resource->loadFromFile(Path.string());
		Debug::Assert(result, "Failed to load Image");
		return resource;
	}
};
}
