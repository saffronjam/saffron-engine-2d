#pragma once

#include <SFML/Graphics/Font.hpp>

#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class FontStore : public ResourceStore<sf::Font>
{
public:
	static auto Get(const Path& Path, bool copy = false) -> Shared<sf::Font>
	{
		return Instance().Fetch(Path, copy);
	}

private:
	auto Copy(const Shared<sf::Font>& value) -> Shared<sf::Font> override
	{
		return CreateShared<sf::Font>(*value);
	}

	auto Location() -> Path override
	{
		return "Assets/Fonts/";
	}

private:
	auto Load(Path Path) -> Shared<sf::Font> override
	{
		auto resource = CreateShared<sf::Font>();
		const auto result = resource->loadFromFile(Path.string());
		Debug::Assert(result, "Failed to load Font");
		return resource;
	}
};
}
