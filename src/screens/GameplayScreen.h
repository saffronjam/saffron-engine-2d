#pragma once

#include "IScreen.h"
#include "IApp.h"

#include "Camera.h"

class GameplayScreen : public IScreen
{
public:
    GameplayScreen();
    ~GameplayScreen();

    virtual void Build() override;

    virtual void Destroy() override;

    virtual void OnEntry() override;

    virtual void OnExit() override;

    virtual void Update() override;

    virtual void Draw() override;

    virtual int GetNextScreenIndex() const override;

    virtual int GetPreviousScreenIndex() const override;

private:
    void CheckInput();

private:
    Camera m_camera;
    sf::Sound sound;
};