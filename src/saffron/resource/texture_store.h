#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "saffron/resource/resource_store.h"

namespace saffron
{
class TextureStore : public ResourceStore<sf::Texture>
{
public:
	static auto Get(const std::filesystem::path& path, bool copy) -> std::shared_ptr<sf::Texture>
	{
		return Instance().Fetch(path, copy);
	}

	static auto TryGet(const std::filesystem::path& path, bool copy) -> Result<std::shared_ptr<sf::Texture>>
	{
		return Instance().TryFetch(path, copy);
	}

private:
	auto TryCopy(const std::shared_ptr<sf::Texture>& value) -> Result<std::shared_ptr<sf::Texture>> override
	{
		return std::make_shared<sf::Texture>(*value);
	}

	auto Location() -> std::filesystem::path override
	{
		return "assets/textures/";
	}

private:
	auto TryLoad(std::filesystem::path path) -> Result<std::shared_ptr<sf::Texture>> override
	{
		auto resource = std::make_shared<sf::Texture>();
		const auto result = resource->loadFromFile(path.string());
		if (!result)
		{
			return std::unexpected(Error{ErrorCode::Resource, "Failed to load texture " + path.string()});
		}
		return resource;
	}
};
}
