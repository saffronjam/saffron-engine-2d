#pragma once

#include <Box2D/Box2D.h>

#include "IScreen.h"
#include "Camera.h"
#include "BoxEntity.h"
#include "CircleEntity.h"

class AppClient;

class ClientMainScreen : public IScreen
{
public:
    ClientMainScreen(AppClient &parent);
    ~ClientMainScreen();

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
    AppClient &m_parent;

    std::vector<BoxEntity> m_boxes;
    std::vector<CircleEntity> m_circles;

    BoxEntity m_mouseCollider;
};