#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "TextureMgr.h"
#include "Camera.h"
#include "PhysicsMgr.h"
#include "CircleCollider.h"

class CircleEntity
{
public:
    CircleEntity(const sf::Vector2f &position)
        : m_sprite(20.0f, 50.0f),
          m_boxCollider(CircleCollider::Create(position, 20.0f))
    {
        m_sprite.setOrigin(Lib::Mid(m_sprite.getLocalBounds()));
        m_sprite.setTexture(TextureMgr::Get("res/sample.png"));
    }

    void Draw()
    {
        // sf::FloatRect rect = m_boxCollider->GetRect();
        m_sprite.setPosition(vl::ConvertTo<sf::Vector2f>(m_boxCollider->GetBody()->GetPosition()));
        m_sprite.setRotation(Lib::ToDegress(m_boxCollider->GetBody()->GetAngle()));
        // m_sprite.setScale(rect.width / m_sprite.getLocalBounds().width, rect.height / m_sprite.getLocalBounds().height);
        Camera::Draw(m_sprite);
    }

    b2Body *GetBody() { return m_boxCollider->GetBody(); }

private:
    sf::CircleShape m_sprite;
    CircleCollider::Ptr m_boxCollider;
};
