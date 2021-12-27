#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class TextureStore : public ResourceStore<sf::Texture>
{
public:
	static auto Get(const std::filesystem::path& path, bool copy = false) -> std::shared_ptr<sf::Texture>
	{
		return Instance().Fetch(path, copy);
	}

private:
	auto Copy(const std::shared_ptr<sf::Texture>& value) -> std::shared_ptr<sf::Texture> override
	{
		return std::make_shared<sf::Texture>(*value);
	}

	auto Location() -> std::filesystem::path override
	{
		return "Assets/Textures/";
	}

private:
	auto Load(std::filesystem::path path) -> std::shared_ptr<sf::Texture> override
	{
		auto resource = std::make_shared<sf::Texture>();
		const auto result = resource->loadFromFile(path.string());
		Debug::Assert(result, "Failed to load Texture");
		return resource;
	}
};
}
