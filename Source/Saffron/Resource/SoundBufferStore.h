#pragma once

#include <SFML/Audio/SoundBuffer.hpp>

#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class SoundBufferStore : public ResourceStore<sf::SoundBuffer>
{
public:
	static auto Get(const std::filesystem::path& path, bool copy) -> std::shared_ptr<sf::SoundBuffer>
	{
		return Instance().Fetch(path, copy);
	}

private:
	auto Copy(const std::shared_ptr<sf::SoundBuffer>& value) -> std::shared_ptr<sf::SoundBuffer> override
	{
		return std::make_shared<sf::SoundBuffer>(*value);
	}

	auto Location() -> std::filesystem::path override
	{
		return "Assets/Sounds/";
	}	

private:
	auto Load(std::filesystem::path path) -> std::shared_ptr<sf::SoundBuffer> override
	{
		auto resource = std::make_shared<sf::SoundBuffer>();
		const auto result = resource->loadFromFile(path.string());
		Debug::Assert(result, "Failed to load SoundBuffer");
		return resource;
	}
};
}
