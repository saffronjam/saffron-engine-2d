#pragma once

#include <SFML/Audio/Sound.hpp>

#include "saffron/resource/resource_store.h"
#include "saffron/resource/sound_buffer_store.h"

namespace saffron
{
class SoundStore : public ResourceStore<sf::Sound>
{
public:
	static auto Get(const std::filesystem::path& path, bool copy) -> std::shared_ptr<sf::Sound>
	{
		return Instance().Fetch(path, copy);
	}

	static auto TryGet(const std::filesystem::path& path, bool copy) -> Result<std::shared_ptr<sf::Sound>>
	{
		return Instance().TryFetch(path, copy);
	}

private:
	auto TryCopy(const std::shared_ptr<sf::Sound>& value) -> Result<std::shared_ptr<sf::Sound>> override
	{
		return std::make_shared<sf::Sound>(*value);
	}

	auto Location() -> std::filesystem::path override
	{
		return "assets/sounds/";
	}

private:
	auto TryLoad(std::filesystem::path path) -> Result<std::shared_ptr<sf::Sound>> override
	{
		auto soundBufferResult = SoundBufferStore::TryGet(path.filename(), false);
		if (!soundBufferResult)
		{
			return std::unexpected(soundBufferResult.error());
		}
		const auto soundBufferResource = *soundBufferResult;
		SoundBufferStore::Instance().Persist(path, soundBufferResource);
		
		auto soundResource = std::make_shared<sf::Sound>(*soundBufferResource);
		return soundResource;
	}
};
}
