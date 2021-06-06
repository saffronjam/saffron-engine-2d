#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

#include "Saffron/Interface/IResourceStore.h"

namespace Se
{
class SoundBufferStore : public IResourceStore<sf::SoundBuffer>
{
public:
	SoundBufferStore() = default;
	SoundBufferStore(const SoundBufferStore&) = delete;
	auto operator()(const SoundBufferStore&) -> const SoundBufferStore& = delete;

	// Returns pointer resource from cache, if not existing, call Load();
	static auto Get(const std::string& filepath) -> sf::SoundBuffer*
	{
		if (_resources.find(filepath) == _resources.end())
		{
			Load(filepath);
		}
		return &_resources[filepath];
	}

	// Returns copy of resource from cache, if not existing, call Load();
	static auto GetCopy(const std::string& filepath) -> const sf::SoundBuffer&
	{
		if (_resources.find(filepath) == _resources.end())
		{
			Load(filepath);
		}
		return _resources[filepath];
	}

	// Load resource into memory
	static void Load(const std::string& filepath)
	{
		sf::SoundBuffer resource;
		if (!resource.loadFromFile(filepath))
		{
			{
				char buf[200];
				sprintf(buf, "Failed to load soundbuffer: %s", filepath.c_str());
				throw Exception(__LINE__, __FILE__, buf);
			}
		}
		_resources.emplace(std::make_pair(filepath, resource));
	}
};
}
