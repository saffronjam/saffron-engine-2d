#include "AppResources.hpp"
#include "Core/Application.hpp"
#include "Core/AllSFML.hpp"

void AppResources::Update()
{
    sf::Sound test;
    test.setBuffer(app.audioMgr.Get(SoundTest));

    if (app.input.Get(KeyP))
        test.play();
}