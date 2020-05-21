#include "App.h"

void App::OnInit()
{
}

void App::OnExit()
{
}

void App::AddScreens()
{
    m_gameplayScreen = std::make_unique<GameplayScreen>();
    m_screenList->AddScreen(m_gameplayScreen.get());
    m_screenList->SetCurrentScreen(m_gameplayScreen->GetScreenIndex());
}