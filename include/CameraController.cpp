#include "CameraController.hpp"

#include "Math.hpp"

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
        if (!m_engaged)
        {
            m_lastPos = (sf::Vector2f)sf::Mouse::getPosition(m_gfx.GetRenderWindow());
        }
        m_engaged = true;
    }
    else
    {
        m_engaged = false;
    }
    if (m_engaged)
    {
        const auto curPos = (sf::Vector2f)sf::Mouse::getPosition(m_gfx.GetRenderWindow());
        auto delta = curPos - m_lastPos;
        if (vf::GetVectorLengthSq(delta) > 0.0f)
        {
            delta = -delta;
            // up down etc. depends on rotation of the camera
            const float cosTheta = cos((-m_camera.GetAngle() * PI) / 180);
            const float sinTheta = sin((-m_camera.GetAngle() * PI) / 180);
            const float new_x = delta.x * cosTheta - delta.y * sinTheta;
            delta.y = delta.x * sinTheta + delta.y * cosTheta;
            delta.x = new_x;

            m_camera.MoveBy(delta / vf::GetVectorLength(m_camera.GetZoom()));
            m_lastPos = curPos;
        }
    }
}