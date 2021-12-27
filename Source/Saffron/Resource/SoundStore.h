#pragma once

#include <SFML/Audio/Sound.hpp>

#include "Saffron/Resource/ResourceStore.h"
#include "Saffron/Resource/SoundBufferStore.h"

namespace Se
{
class SoundStore : public ResourceStore<sf::Sound>
{
public:
	static auto Get(const std::filesystem::path& path, bool copy) -> std::shared_ptr<sf::Sound>
	{
		return Instance().Fetch(path, copy);
	}

private:
	auto Copy(const std::shared_ptr<sf::Sound>& value) -> std::shared_ptr<sf::Sound> override
	{
		return std::make_shared<sf::Sound>(*value);
	}

	auto Location() -> std::filesystem::path override
	{
		return "Assets/Sounds/";
	}

private:
	auto Load(std::filesystem::path path) -> std::shared_ptr<sf::Sound> override
	{
		const auto soundBufferResource = SoundBufferStore::Get(path.filename(), false);
		SoundBufferStore::Instance().Persist(path, soundBufferResource);
		
		auto soundResource = std::make_shared<sf::Sound>(*soundBufferResource);
		return soundResource;
	}
};
}
