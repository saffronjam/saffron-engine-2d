#pragma once

#include <SFML/Audio/Sound.hpp>

#include "Saffron/Resource/ResourceStore.h"
#include "Saffron/Resource/SoundBufferStore.h"

namespace Se
{
class SoundStore : public ResourceStore<sf::Sound>
{
public:
	static auto Get(const Path& path, bool copy) -> Shared<sf::Sound>
	{
		return Instance().Fetch(path, copy);
	}

private:
	auto Copy(const Shared<sf::Sound>& value) -> Shared<sf::Sound> override
	{
		return CreateShared<sf::Sound>(*value);
	}

	auto Location() -> Path override
	{
		return "Assets/Sounds/";
	}

private:
	auto Load(Path path) -> Shared<sf::Sound> override
	{
		const auto soundBufferResource = SoundBufferStore::Get(path.filename(), false);
		SoundBufferStore::Instance().Persist(path, soundBufferResource);
		
		auto soundResource = CreateShared<sf::Sound>(*soundBufferResource);
		return soundResource;
	}
};
}
