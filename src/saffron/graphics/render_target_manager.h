#pragma once

#include "saffron/base.h"
#include "saffron/graphics/controllable_render_texture.h"

namespace saffron
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
