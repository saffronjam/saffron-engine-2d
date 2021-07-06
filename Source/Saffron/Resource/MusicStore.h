#pragma once

#include <SFML/Audio/Music.hpp>

#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class MusicStore : public ResourceStore<sf::Music>
{
public:
	static auto Get(const Path& Path) -> Shared<sf::Music>
	{
		return Instance().Fetch(Path, false);
	}

private:
	auto Location() -> Path override
	{
		return "Assets/Sounds/";
	}

private:
	auto Load(Path Path) -> Shared<sf::Music> override
	{
		auto resource = CreateShared<sf::Music>();
		const auto result = resource->openFromFile(Path.string());
		Debug::Assert(result, "Failed to load Music");
		return resource;
	}
};
}
