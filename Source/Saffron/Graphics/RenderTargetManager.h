#pragma once

#include "Saffron/Base.h"
#include "Saffron/Graphics/ControllableRenderTexture.h"

namespace Se
{
class RenderTargetManager : Singleton<RenderTargetManager>
{
public:
	RenderTargetManager();
	
	static void Add(ControllableRenderTexture* target);
	static void Remove(ControllableRenderTexture* target);

	void ClearAll();
	void DisplayAll();

private:
	std::vector<ControllableRenderTexture*> _targets;
};
}
