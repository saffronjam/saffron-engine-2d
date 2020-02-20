#pragma once
#include "Camera.hpp"
#include "Input.hpp"
#include "UI.hpp"
#include "../Arth/Random.hpp"
#include "../Arth/Math.hpp"
#include "AppResources.hpp"

class Application
{

public:
	Application(Graphics &gfx, Camera &camera, Input &input, sf::Time &dt);
	void Go();

private:
	void UpdateLogic();
	void RenderFrame();

private:
	Graphics &m_gfx;
	Camera &m_camera;
	Input &m_input;
	sf::Time &m_dt;

	AppResources m_resources;
	UI m_ui;
	sf::Vector2f m_tofollow;

public:
	friend class AppResources;
};
