#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include "saffron/resource/resource_store.h"

namespace saffron
{
class SoundBufferStore : public ResourceStore<sf::SoundBuffer>
{
public:
	static auto Get(const std::filesystem::path& path, bool copy) -> std::shared_ptr<sf::SoundBuffer>
	{
		return Instance().Fetch(path, copy);
	}

	static auto TryGet(const std::filesystem::path& path, bool copy) -> Result<std::shared_ptr<sf::SoundBuffer>>
	{
		return Instance().TryFetch(path, copy);
	}

private:
	auto TryCopy(const std::shared_ptr<sf::SoundBuffer>& value) -> Result<std::shared_ptr<sf::SoundBuffer>> override
	{
		return std::make_shared<sf::SoundBuffer>(*value);
	}

	auto Location() -> std::filesystem::path override
	{
		return "assets/sounds/";
	}	

private:
	auto TryLoad(std::filesystem::path path) -> Result<std::shared_ptr<sf::SoundBuffer>> override
	{
		auto resource = std::make_shared<sf::SoundBuffer>();
		const auto result = resource->loadFromFile(path.string());
		if (!result)
		{
			return std::unexpected(Error{ErrorCode::Resource, "Failed to load sound buffer " + path.string()});
		}
		return resource;
	}
};
}
