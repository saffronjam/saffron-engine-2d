#include "CameraController.hpp"

#include "Math.hpp"
#include "InputUtility.hpp"

CameraController::CameraController(Graphics &gfx, Camera &camera, InputUtility &iu)
    : m_gfx(gfx),
      m_camera(camera),
      m_iu(iu),
      m_zoomFactor(1.05f),
      m_rotationSpeed(30)
{
}

void CameraController::Update(sf::Time dt)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
    {
        m_camera.SetAngle(m_camera.GetAngle() + m_rotationSpeed * dt.asSeconds());
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
    {
        m_camera.SetAngle(m_camera.GetAngle() - m_rotationSpeed * dt.asSeconds());
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        m_engaged = true;
        sf::Vector2f delta = (sf::Vector2f)m_iu.GetLastMouseSweep();
        delta = -delta;
        const float cosTheta = cos((-m_camera.GetAngle() * Math::Constants::PI) / 180);
        const float sinTheta = sin((-m_camera.GetAngle() * Math::Constants::PI) / 180);
        const float new_x = delta.x * cosTheta - delta.y * sinTheta;
        delta.y = delta.x * sinTheta + delta.y * cosTheta;
        delta.x = new_x;

        m_camera.MoveBy(delta / vf::Length(m_camera.GetZoom()));
    }
    else
    {
        m_engaged = false;
    }
}