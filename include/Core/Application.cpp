#include "Application.hpp"
#include <stdlib.h>
#include <time.h>

Application::Application(Graphics &gfx, Camera &camera, Input &input, sf::Time &dt)
	: m_gfx(gfx),
	  m_camera(camera),
	  m_input(input),
	  m_dt(dt),
	  m_resources(*this),
	  m_ui(m_gfx),
	  m_tofollow(0.0f, 0.0f)
{
	srand(time(NULL));
	m_camera.SetToFollow(&m_tofollow);
}

void Application::Go()
{
	m_gfx.ClearFrame();
	UpdateLogic();
	RenderFrame();
	m_gfx.EndFrame();
}

void Application::UpdateLogic()
{
	m_camera.Update(m_dt);
	m_ui.Update(m_dt);

	m_resources.Update();
}

void Application::RenderFrame()
{
	m_resources.Draw();
}