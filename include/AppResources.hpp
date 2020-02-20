#pragma once

class AppResources
{
public:
    AppResources(class Application &app);
    ~AppResources();
    void Draw();
    void Update();

private:
    class Application &app;

private:
    //Write here
};