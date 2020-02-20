#include "Application.hpp"
#include <stdlib.h>
#include <time.h>

Application::Application(Graphics &gfx, Camera &camera, Input &input, sf::Time &dt)
	: gfx(gfx),
	  camera(camera),
	  input(input),
	  dt(dt),
	  resources(*this),
	  ui(gfx),
	  tofollow(0.0f, 0.0f)
{
	srand(time(NULL));
	camera.SetToFollow(&tofollow);
}

void Application::Go()
{
	gfx.ClearFrame();
	UpdateLogic();
	RenderFrame();
	gfx.EndFrame();
}

void Application::UpdateLogic()
{
	camera.Update(dt);
	ui.Update(dt);

	resources.Update();
}

void Application::RenderFrame()
{
	resources.Draw();
}