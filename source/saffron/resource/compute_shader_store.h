#pragma once

#include "saffron/graphics/compute_shader.h"
#include "saffron/resource/resource_store.h"

namespace saffron
{
class ComputeShaderStore : public ResourceStore<ComputeShader>
{
public:
	ComputeShaderStore() = default;

	static auto Get(const  std::filesystem::path& path) -> std::shared_ptr<ComputeShader>
	{
		return Instance().Fetch(path, false);
	}

	static auto TryGet(const std::filesystem::path& path) -> Result<std::shared_ptr<ComputeShader>>
	{
		return Instance().TryFetch(path, false);
	}

private:
	auto Location() ->  std::filesystem::path override
	{
		return "assets/shaders/";
	}

private:
	auto TryLoad(std::filesystem::path path) -> Result<std::shared_ptr<ComputeShader>> override
	{
		auto resource = std::make_shared<ComputeShader>();
		auto result = resource->TryLoadFromFile(path);
		if (!result)
		{
			return std::unexpected(result.error());
		}
		return resource;
	}
};
}
