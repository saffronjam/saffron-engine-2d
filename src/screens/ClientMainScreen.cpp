#include "ClientMainScreen.h"
#include "AppClient.h"

ClientMainScreen::ClientMainScreen(AppClient &parent)
    : m_parent(parent)
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
    LightningMgr::SetAmbientLight(sf::Color(100, 100, 100));
}

void ClientMainScreen::OnExit()
{
}

void ClientMainScreen::Update()
{
    if (Mouse::IsPressed(sf::Mouse::Button::Left))
    {
        LightEntity lEntity;
        lEntity.direction = vl::Unit(Camera::ScreenToWorld(Mouse::GetPos()));
        lEntity.speed = 200.0f;
        lEntity.light = SpotLight::Create(sf::Vector2f(0.0f, 0.0f), -lEntity.direction, 40.0f, 120.0f, Random::Color());
        m_lights.push_back(lEntity);
        LightningMgr::AddLight(lEntity.light);
    }

    for (auto &lEnttiy : m_lights)
    {
        lEnttiy.light->SetPosition(lEnttiy.light->GetPosition() + lEnttiy.direction * lEnttiy.speed * Clock::Delta().asSeconds());
    }

    if (Keyboard::IsPressed(sf::Keyboard::Space))
    {
        LightningMgr::RemoveLight(m_lights.front().light);
        m_lights.erase(m_lights.begin());
    }

    sf::CircleShape circShape;
    circShape.setPosition(Camera::ScreenToWorld(Mouse::GetPos()));
    circShape.setRadius(150.0f);
    circShape.setFillColor(sf::Color::Red);
    LightningMgr::AddOccluder(circShape, sf::FloatRect(circShape.getPosition(), sf::Vector2f(300.0f, 300.0f)));
}

void ClientMainScreen::Draw()
{
    sf::RectangleShape rectShape;
    rectShape.setPosition(0.0f, 0.0f);
    rectShape.setSize(Lib::ConvertTo<float>(Window::GetSize()));
    rectShape.setTexture(TextureMgr::Get("res/sample_image.png"));
    Window::Draw(rectShape);
}

int ClientMainScreen::GetNextScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}

int ClientMainScreen::GetPreviousScreenIndex() const
{
    return SCREENINDEX_NO_SCREEN;
}