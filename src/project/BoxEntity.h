#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "TextureMgr.h"
#include "Camera.h"
#include "PhysicsMgr.h"
#include "BoxCollider.h"

class BoxEntity
{
public:
    BoxEntity(const sf::Vector2f &position)
        : m_sprite(TextureMgr::GetCopy("res/sample.png")),
          m_boxCollider(BoxCollider::Create(position, sf::Vector2f(20.0f, 20.0f)))
    {
        m_sprite.setOrigin(Lib::Mid(m_sprite.getLocalBounds()));
    }

    void Draw()
    {
        sf::FloatRect rect = m_boxCollider->GetRect();
        m_sprite.setPosition(rect.left, rect.top);
        m_sprite.setRotation(Lib::ToDegress(m_boxCollider->GetBody()->GetAngle()));
        m_sprite.setScale(rect.width / m_sprite.getLocalBounds().width, rect.height / m_sprite.getLocalBounds().height);
        Camera::Draw(m_sprite);
    }

    b2Body *GetBody() { return m_boxCollider->GetBody(); }

private:
    sf::Sprite m_sprite;
    BoxCollider::Ptr m_boxCollider;
};
