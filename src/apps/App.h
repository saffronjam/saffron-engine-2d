#pragma once

#include "memory"

#include "IApp.h"
#include "GameplayScreen.h"

class App : public IApp
{
public:
    virtual void OnInit() override;
    virtual void OnExit() override;
    virtual void AddScreens() override;

private:
    std::unique_ptr<GameplayScreen> m_gameplayScreen;
};