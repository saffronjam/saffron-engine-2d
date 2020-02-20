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
	Graphics &gfx;
	Camera &camera;
	Input &input;
	sf::Time &dt;

	AppResources resources;
	UI ui;
	sf::Vector2f tofollow;

public:
	friend class AppResources;
};
