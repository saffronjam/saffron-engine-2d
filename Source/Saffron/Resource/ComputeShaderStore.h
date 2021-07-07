#pragma once

#include "Saffron/Graphics/ComputeShader.h"
#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class ComputeShaderStore : public ResourceStore<ComputeShader>
{
public:
	ComputeShaderStore() = default;

	static auto Get(const Path& path) -> Shared<ComputeShader>
	{
		return Instance().Fetch(path, false);
	}

private:
	auto Location() -> Path override
	{
		return "Assets/Shaders/";
	}

private:
	auto Load(Path path) -> Shared<ComputeShader> override
	{
		auto resource = CreateShared<ComputeShader>();
		const auto result = resource->LoadFromFile(path.string());
		Debug::Assert(result, "Failed to load ComputeShader");
		return resource;
	}
};
}
