#pragma once

#include <SFML/Audio/Music.hpp>

#include "saffron/resource/resource_store.h"

namespace saffron
{
class MusicStore : public ResourceStore<sf::Music>
{
public:
	static auto Get(const std::filesystem::path& path) -> std::shared_ptr<sf::Music>
	{
		return Instance().Fetch(path, false);
	}

	static auto TryGet(const std::filesystem::path& path) -> Result<std::shared_ptr<sf::Music>>
	{
		return Instance().TryFetch(path, false);
	}

private:
	auto Location() -> std::filesystem::path override
	{
		return "assets/sounds/";
	}

private:
	auto TryLoad(std::filesystem::path path) -> Result<std::shared_ptr<sf::Music>> override
	{
		auto resource = std::make_shared<sf::Music>();
		const auto result = resource->openFromFile(path.string());
		if (!result)
		{
			return std::unexpected(Error{ErrorCode::Resource, "Failed to load music " + path.string()});
		}
		return resource;
	}
};
}
