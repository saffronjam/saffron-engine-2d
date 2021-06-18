#include "SaffronPCH.h"

#include <ranges>

#include "Saffron/Graphics/RenderTargetManager.h"

namespace Se
{
RenderTargetManager::RenderTargetManager() :
	SingleTon(this)
{
}

void RenderTargetManager::Add(ControllableRenderTexture* target)
{
	Instance()._targets.push_back(target);
}

void RenderTargetManager::Remove(ControllableRenderTexture* target)
{
	auto& targets = Instance()._targets;

	const auto result = std::ranges::find(targets, target);
	if (result != targets.end())
	{
		targets.erase(result);
	}
}

void RenderTargetManager::ClearAll()
{
	for (auto* target : _targets)
	{
		if (target->IsEnabled())
		{
			target->RenderTexture().clear(target->ClearColor());
		}
	}
}

void RenderTargetManager::DisplayAll()
{
	for (auto* target : _targets)
	{
		if (target->IsEnabled())
		{
			target->RenderTexture().display();
		}
	}
}
}
