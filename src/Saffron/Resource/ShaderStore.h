#pragma once

#include <SFML/Graphics/Shader.hpp>

#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class ShaderStore : public ResourceStore<sf::Shader>
{
public:
	struct ShaderStoreEntry
	{
	};

public:
	static auto Get(const Path& vertexShader, const Path& pixelShader) -> Shared<sf::Shader>
	{
		const Path concat = vertexShader.string() + ConcatToken + pixelShader.string();
		return Instance().Fetch(concat, false);
	}

	static auto Get(const Path& path, sf::Shader::Type type) -> Shared<sf::Shader>
	{
		switch (type)
		{
		case sf::Shader::Type::Vertex:
		{
			const Path concat = path.string() + VertexToken;
			return Instance().Fetch(concat, false);
		}
		case sf::Shader::Type::Fragment:
		{
			const Path concat = path.string() + PixelToken;
			return Instance().Fetch(concat, false);
		}
		default:
		{
			Debug::Break("Shader type not supported in store");
			return nullptr;
		}
		}
	}

private:
	auto Location() -> Path override
	{
		return "res/Shaders/";
	}

private:
	auto Load(Path Path) -> Shared<sf::Shader> override
	{
		const auto fpString = Path.string();

		const auto concatTokenIndex = fpString.find(ConcatToken);
		auto resource = CreateShared<sf::Shader>();
		bool result = false;


		// If vertex shader + pixel shader
		if (concatTokenIndex == String::npos)
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

		Debug::Assert(result, "Failed to load ComputeShader");
		return resource;
	}

	inline static const String ConcatToken = "%%";
	inline static const String VertexToken = "%V%";
	inline static const String PixelToken = "%P%";
	static constexpr size_t TypeTokenLength = 3;
};
}
