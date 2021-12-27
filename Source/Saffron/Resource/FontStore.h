#pragma once

#include <SFML/Graphics/Font.hpp>

#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class FontStore : public ResourceStore<sf::Font>
{
public:
	static auto Get(const std::filesystem::path& path, bool copy = false) -> std::shared_ptr<sf::Font>
	{
		return Instance().Fetch(path, copy);
	}

private:
	auto Copy(const std::shared_ptr<sf::Font>& value) -> std::shared_ptr<sf::Font> override
	{
		return std::make_shared<sf::Font>(*value);
	}

	auto Location() -> std::filesystem::path override
	{
		return "Assets/Fonts/";
	}

private:
	auto Load(std::filesystem::path path) -> std::shared_ptr<sf::Font> override
	{
		auto resource = std::make_shared<sf::Font>();
		const auto result = resource->loadFromFile(path.string());
		Debug::Assert(result, "Failed to load Font");
		return resource;
	}
};
}
