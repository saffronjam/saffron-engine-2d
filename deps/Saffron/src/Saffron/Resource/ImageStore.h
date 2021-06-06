#pragma once

#include <SFML/Graphics/Image.hpp>

#include "Saffron/Interface/IResourceStore.h"

namespace Se
{
class ImageStore : public IResourceStore<sf::Image>
{
public:
	ImageStore() = default;
	ImageStore(const ImageStore&) = delete;
	auto operator()(const ImageStore&) -> const ImageStore& = delete;

	static auto Get(const std::string& filepath) -> sf::Image*
	{
		if (_resources.find(filepath) == _resources.end())
		{
			Load(filepath);
		}
		return &_resources[filepath];
	}

	// Returns copy of resource from cache, if not existing, call Load();
	static auto GetCopy(const std::string& filepath) -> const sf::Image&
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
		sf::Image resource;
		if (!resource.loadFromFile(filepath))
		{
			{
				char buf[200];
				sprintf(buf, "Failed to load font: %s", filepath.c_str());
				throw Exception(__LINE__, __FILE__, buf);
			}
		}
		_resources.emplace(std::make_pair(filepath, resource));
	}
};
}
