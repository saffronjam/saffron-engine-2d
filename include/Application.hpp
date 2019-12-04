#pragma once
#include "Camera.hpp"
#include "CameraController.hpp"
#include "InputUtility.hpp"

class Application
{

public:
	Application(sf::Time &dt);
	void Go();

private:
	void HandleEvents();
	void UpdateLogic();
	void RenderFrame();

public:
	Graphics m_gfx;

private:
	sf::Time &m_dt;
	sf::Event m_event;
	InputUtility m_iu;

	Camera m_camera;
	CameraController m_camera_controller;
	sf::Vector2f m_camera_follow;
};
