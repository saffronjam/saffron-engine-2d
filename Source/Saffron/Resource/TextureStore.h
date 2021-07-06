#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class TextureStore : public ResourceStore<sf::Texture>
{
public:
	static auto Get(const Path& Path, bool copy = false) -> Shared<sf::Texture>
	{
		return Instance().Fetch(Path, copy);
	}

private:
	auto Copy(const Shared<sf::Texture>& value) -> Shared<sf::Texture> override
	{
		return CreateShared<sf::Texture>(*value);
	}

	auto Location() -> Path override
	{
		return "Assets/Textures/";
	}

private:
	auto Load(Path Path) -> Shared<sf::Texture> override
	{
		auto resource = CreateShared<sf::Texture>();
		const auto result = resource->loadFromFile(Path.string());
		Debug::Assert(result, "Failed to load Texture");
		return resource;
	}
};
}
