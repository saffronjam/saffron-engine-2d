#include "include/Core/Application.hpp"
#include "include/Core/EventMgr.hpp"
#include <SFML/System/Clock.hpp>

#include "include/Core/Graphics.hpp"

int main()
{

	Graphics m_gfx;
	Input m_input;
	Camera m_camera(m_gfx, m_input);
	EventMgr m_eventMgr(m_gfx.GetRenderWindow(), m_input);
	sf::Clock m_application_clock;
	sf::Time m_dt;
	Application m_application(m_gfx, m_camera, m_input, m_dt);

	m_gfx.SetCamera(&m_camera);

	while (m_gfx.GetRenderWindow().isOpen())
	{
		m_eventMgr.PollAll();
		if (m_input.Get(Code::EventQuit) || m_input.Get(Code::KeyEsc))
			m_gfx.GetRenderWindow().close();
		m_application.Go();
		m_dt = m_application_clock.getElapsedTime();
		m_application_clock.restart();
	}
	return EXIT_SUCCESS;
}