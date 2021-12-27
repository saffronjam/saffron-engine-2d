#pragma once

#include <SFML/Audio/Music.hpp>

#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class MusicStore : public ResourceStore<sf::Music>
{
public:
	static auto Get(const std::filesystem::path& path) -> std::shared_ptr<sf::Music>
	{
		return Instance().Fetch(path, false);
	}

private:
	auto Location() -> std::filesystem::path override
	{
		return "Assets/Sounds/";
	}

private:
	auto Load(std::filesystem::path path) -> std::shared_ptr<sf::Music> override
	{
		auto resource = std::make_shared<sf::Music>();
		const auto result = resource->openFromFile(path.string());
		Debug::Assert(result, "Failed to load Music");
		return resource;
	}
};
}
