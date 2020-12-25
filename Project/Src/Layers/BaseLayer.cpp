#include "BaseLayer.h"

namespace Se
{
BaseLayer::BaseLayer() :
	_controllableRenderTexture(100, 100),
	_scene(&_controllableRenderTexture, &_camera),
	_viewportPane("Viewport", _controllableRenderTexture)
{
}

void BaseLayer::OnAttach(std::shared_ptr<BatchLoader> &loader)
{
	_viewportPane.GetSignal(ViewportPane::Signals::OnWantRenderTargetResize).Connect(
		[this](const sf::Vector2f &size)
		{
			OnRenderTargetResize(size);
		}
	);
	RenderTargetManager::Add(&_controllableRenderTexture);
}

void BaseLayer::OnDetach()
{
}

void BaseLayer::OnFirstFrame()
{
	_viewportPane.OnGuiRender();
	_scene.SetViewportSize(_viewportPane.GetViewportSize());
}

void BaseLayer::OnUpdate()
{
	_scene.OnUpdate();
}

void BaseLayer::OnGuiRender()
{
	_dockSpace.OnGuiRender();
	_camera.OnGuiRender();
	_terminal.OnGuiRender();
	_viewportPane.OnGuiRender();
	Application::Get().OnGuiRender();
}

void BaseLayer::OnRenderTargetResize(const sf::Vector2f &newSize)
{
	_controllableRenderTexture.GetRenderTexture().create(newSize.x, newSize.y);
	_scene.SetViewportSize(newSize);
	_camera.SetViewportSize(newSize);
}
}
