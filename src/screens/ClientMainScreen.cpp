#include "ClientMainScreen.h"
#include "AppClient.h"

ClientMainScreen::ClientMainScreen(AppClient &parent)
    : m_parent(parent),
      m_mouseCollider(sf::Vector2f(-100.0f, 0.0f))
{
}

ClientMainScreen::~ClientMainScreen()
{
}

void ClientMainScreen::Build()
{
}

void ClientMainScreen::Destroy()
{
}

void ClientMainScreen::OnEntry()
{
    FPSLimiter::SetDesiredFPS(144.0f);
    LightningMgr::SetAmbientLight(sf::Color(255, 255, 255));

    const int nBoxes = 25;
    for (int i = 0; i < nBoxes; i++)
    {
        m_boxes.push_back(BoxEntity(Random::Vec2(-50.0f, -50.0f, 50.0f, 50.0f)));
    }
    const int nCircles = 25;
    for (int i = 0; i < nCircles; i++)
    {
        m_circles.push_back(CircleEntity(Random::Vec2(-50.0f, -50.0f, 50.0f, 50.0f)));
    }
}

void ClientMainScreen::OnExit()
{
}

void ClientMainScreen::Update()
{
    m_mouseCollider.GetBody()->ApplyLinearImpulse(vl::ConvertTo<b2Vec2>((Camera::ScreenToWorld(Mouse::GetPos()))) - m_mouseCollider.GetBody()->GetPosition(),
                                                  m_mouseCollider.GetBody()->GetPosition(),
                                                  true);
}

void ClientMainScreen::Draw()
{
    for (auto &box : m_boxes)
        box.Draw();
    for (auto &circle : m_circles)
        circle.Draw();

    m_mouseCollider.Draw();
}

int ClientMainScreen::GetNextScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}

int ClientMainScreen::GetPreviousScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}