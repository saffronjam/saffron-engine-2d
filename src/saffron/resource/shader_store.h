#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "saffron/resource/resource_store.h"

namespace saffron
{
class ShaderStore : public ResourceStore<sf::Shader>
{
public:
	struct ShaderStoreEntry
	{
	};

public:
	static auto Get(const std::filesystem::path& vertexShader, const std::filesystem::path& pixelShader) -> std::shared_ptr<sf::Shader>
	{
		const std::filesystem::path concat = vertexShader.string() + ConcatToken + pixelShader.string();
		return Instance().Fetch(concat, false);
	}

	static auto TryGet(const std::filesystem::path& vertexShader, const std::filesystem::path& pixelShader) -> Result<std::shared_ptr<sf::Shader>>
	{
		const std::filesystem::path concat = vertexShader.string() + ConcatToken + pixelShader.string();
		return Instance().TryFetch(concat, false);
	}

	static auto Get(const std::filesystem::path& path, sf::Shader::Type type) -> std::shared_ptr<sf::Shader>
	{
		switch (type)
		{
		case sf::Shader::Type::Vertex:
		{
			const std::filesystem::path concat = path.string() + VertexToken;
			return Instance().Fetch(concat, false);
		}
		case sf::Shader::Type::Fragment:
		{
			const std::filesystem::path concat = path.string() + PixelToken;
			return Instance().Fetch(concat, false);
		}
		default:
		{
			Log::CoreError("Shader type not supported in store");
			return nullptr;
		}
		}
	}

	static auto TryGet(const std::filesystem::path& path, sf::Shader::Type type) -> Result<std::shared_ptr<sf::Shader>>
	{
		switch (type)
		{
		case sf::Shader::Type::Vertex:
		{
			const std::filesystem::path concat = path.string() + VertexToken;
			return Instance().TryFetch(concat, false);
		}
		case sf::Shader::Type::Fragment:
		{
			const std::filesystem::path concat = path.string() + PixelToken;
			return Instance().TryFetch(concat, false);
		}
		default:
		{
			return std::unexpected(Error{ErrorCode::InvalidArgument, "Shader type not supported in store"});
		}
		}
	}

private:
	auto Location() -> std::filesystem::path override
	{
		return "assets/shaders/";
	}

private:
	auto TryLoad(std::filesystem::path path) -> Result<std::shared_ptr<sf::Shader>> override
	{
		const auto fpString = path.string();

		const auto concatTokenIndex = fpString.find(ConcatToken);
		auto resource = std::make_shared<sf::Shader>();
		bool result = false;


		// If vertex shader + pixel shader
		if (concatTokenIndex == std::string::npos)
		{
			const auto typeToken = fpString.substr(fpString.length() - TypeTokenLength);
			const auto formattedPath = fpString.substr(0, fpString.length() - TypeTokenLength);
			if (typeToken == VertexToken)
			{
				result = resource->loadFromFile(formattedPath, sf::Shader::Type::Vertex);
			}
			else if (typeToken == PixelToken)
			{
				result = resource->loadFromFile(formattedPath, sf::Shader::Type::Fragment);
			}
		}
		else
		{
			const auto pixelShaderPath = fpString.substr(0, concatTokenIndex);
			const auto vertexShaderPath = fpString.substr(concatTokenIndex + ConcatToken.length());
			result = resource->loadFromFile(pixelShaderPath, vertexShaderPath);
		}

		if (!result)
		{
			return std::unexpected(Error{ErrorCode::Resource, "Failed to load shader " + fpString});
		}
		return resource;
	}

	inline static const std::string ConcatToken = "%%";
	inline static const std::string VertexToken = "%V%";
	inline static const std::string PixelToken = "%P%";
	static constexpr size_t TypeTokenLength = 3;
};
}
