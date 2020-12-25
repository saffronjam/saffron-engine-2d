#pragma once

#include <Saffron.h>

namespace Se
{
class BaseLayer : public Layer
{
public:
	BaseLayer();

	void OnAttach(std::shared_ptr<BatchLoader> &loader) override;
	void OnDetach() override;

	void OnFirstFrame();
	void OnUpdate() override;
	void OnGuiRender() override;

	virtual void OnRenderTargetResize(const sf::Vector2f &newSize);

protected:
	ControllableRenderTexture _controllableRenderTexture;
	Camera _camera;
	Scene _scene;
	Terminal _terminal;

private:
	DockSpace _dockSpace;
	ViewportPane _viewportPane;

};

};
