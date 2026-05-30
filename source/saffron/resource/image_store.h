#pragma once

#include <SFML/Graphics/Image.hpp>

#include "saffron/resource/resource_store.h"

namespace saffron
{
class ImageStore : public ResourceStore<sf::Image>
{
public:
	static auto Get(const std::filesystem::path& path, bool copy) -> std::shared_ptr<sf::Image>
	{
		return Instance().Fetch(path, copy);
	}

	static auto TryGet(const std::filesystem::path& path, bool copy) -> Result<std::shared_ptr<sf::Image>>
	{
		return Instance().TryFetch(path, copy);
	}

private:
	auto TryCopy(const std::shared_ptr<sf::Image>& value) -> Result<std::shared_ptr<sf::Image>> override
	{
		auto resource = std::make_shared<sf::Image>();
		resource->copy(*value, 0, 0);
		return resource;
	}

	auto Location() -> std::filesystem::path override
	{
		return "assets/textures/";
	}

private:
	auto TryLoad(std::filesystem::path path) -> Result<std::shared_ptr<sf::Image>> override
	{
		auto resource = std::make_shared<sf::Image>();
		const auto result = resource->loadFromFile(path.string());
		if (!result)
		{
			return std::unexpected(Error{ErrorCode::Resource, "Failed to load image " + path.string()});
		}
		return resource;
	}
};
}
