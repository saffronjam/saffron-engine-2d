#pragma once

#include "Saffron/Graphics/ComputeShader.h"
#include "Saffron/Resource/ResourceStore.h"

namespace Se
{
class ComputeShaderStore : public ResourceStore<ComputeShader>
{
public:
	ComputeShaderStore() = default;

	static auto Get(const Path& Path) -> Shared<ComputeShader>
	{
		return Instance().Fetch(Path, false);
	}

private:
	auto Location() -> Path override
	{
		return "Assets/Shaders/";
	}

private:
	auto Load(Path Path) -> Shared<ComputeShader> override
	{
		auto resource = CreateShared<ComputeShader>();
		const auto result = resource->LoadFromFile(Path.string());
		Debug::Assert(result, "Failed to load ComputeShader");
		return resource;
	}
};
}
