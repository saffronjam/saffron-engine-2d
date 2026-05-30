#pragma once

#include <SFML/Graphics/Font.hpp>

#include "saffron/resource/resource_store.h"

namespace saffron
{
class FontStore : public ResourceStore<sf::Font>
{
public:
	static auto Get(const std::filesystem::path& path, bool copy) -> std::shared_ptr<sf::Font>
	{
		return Instance().Fetch(path, copy);
	}

	static auto TryGet(const std::filesystem::path& path, bool copy) -> Result<std::shared_ptr<sf::Font>>
	{
		return Instance().TryFetch(path, copy);
	}

private:
	auto TryCopy(const std::shared_ptr<sf::Font>& value) -> Result<std::shared_ptr<sf::Font>> override
	{
		return std::make_shared<sf::Font>(*value);
	}

	auto Location() -> std::filesystem::path override
	{
		return "assets/fonts/";
	}

private:
	auto TryLoad(std::filesystem::path path) -> Result<std::shared_ptr<sf::Font>> override
	{
		auto resource = std::make_shared<sf::Font>();
		const auto result = resource->loadFromFile(path.string());
		if (!result)
		{
			return std::unexpected(Error{ErrorCode::Resource, "Failed to load font " + path.string()});
		}
		return resource;
	}
};
}
