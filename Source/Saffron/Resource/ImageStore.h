#pragma once

#include <SFML/Graphics/Image.hpp>

#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class ImageStore : public ResourceStore<sf::Image>
{
public:
	static auto Get(const std::filesystem::path& path, bool copy = false) -> std::shared_ptr<sf::Image>
	{
		return Instance().Fetch(path, copy);
	}

private:
	auto Copy(const std::shared_ptr<sf::Image>& value) -> std::shared_ptr<sf::Image> override
	{
		auto resource = std::make_shared<sf::Image>();
		resource->copy(*value, 0, 0);
		return resource;
	}

	auto Location() -> std::filesystem::path override
	{
		return "Assets/Textures/";
	}

private:
	auto Load(std::filesystem::path path) -> std::shared_ptr<sf::Image> override
	{
		auto resource = std::make_shared<sf::Image>();
		const auto result = resource->loadFromFile(path.string());
		Debug::Assert(result, "Failed to load Image");
		return resource;
	}
};
}
