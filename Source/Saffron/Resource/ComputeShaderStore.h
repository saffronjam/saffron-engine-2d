#pragma once

#include "Saffron/Graphics/ComputeShader.h"
#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class ComputeShaderStore : public ResourceStore<ComputeShader>
{
public:
	ComputeShaderStore() = default;

	static auto Get(const  std::filesystem::path& path) -> std::shared_ptr<ComputeShader>
	{
		return Instance().Fetch(path, false);
	}

private:
	auto Location() ->  std::filesystem::path override
	{
		return "Assets/Shaders/";
	}

private:
	auto Load( std::filesystem::path path) -> std::shared_ptr<ComputeShader> override
	{
		auto resource = std::make_shared<ComputeShader>();
		const auto result = resource->LoadFromFile(path.string());
		Debug::Assert(result, "Failed to load ComputeShader");
		return resource;
	}
};
}
