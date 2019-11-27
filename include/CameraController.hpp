#pragma once

#include "Camera.hpp"

class CameraController
{
public:
    CameraController(Graphics &gfx, Camera &camera)
        : m_gfx(gfx),
          m_camera(camera),
          m_zoomFactor(1.05f),
          m_rotationSpeed(30)
    {
    }

    void Update(sf::Time dt);

    bool GetEngaged() { return m_engaged; }

private:
    Graphics &m_gfx;
    Camera &m_camera;

    sf::Vector2f m_lastPos;
    bool m_engaged;

    float m_zoomFactor;
    float m_rotationSpeed;
};