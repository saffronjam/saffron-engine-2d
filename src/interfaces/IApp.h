#pragma once

#include <memory>

#include "AppThrowMacros.h"
#include "Window.h"
#include "EventMgr.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ScreenList.h"
#include "TextureMgr.h"
#include "FontMgr.h"
#include "MusicMgr.h"
#include "SoundBufferMgr.h"
#include "PacketMgr.h"
#include "IScreen.h"

class IApp : public EventHandler
{
public:
    IApp();
    virtual ~IApp();

    void Run();
    void Exit();

    virtual void OnInit() = 0;
    virtual void OnExit() = 0;
    virtual void AddScreens() = 0;

    TextureMgr &GetTextureMgr() { return m_textureMgr; };
    FontMgr &GetFontMgr() { return m_fontMgr; };
    MusicMgr &GetMusicMgr() { return m_musicMgr; };
    SoundBufferMgr &GetSoundBufferMgr() { return m_soundBufferMgr; };

protected:
    virtual void Update();
    virtual void Draw();

    bool Init();

    virtual void OnEvent(const sf::Event &event) override;

protected:
    std::unique_ptr<ScreenList> m_screenList;
    IScreen *m_currentScreen;
    Window m_window;
    EventMgr m_eventMgr;
    Keyboard m_keyboard;
    Mouse m_mouse;

    TextureMgr m_textureMgr;
    FontMgr m_fontMgr;
    MusicMgr m_musicMgr;
    SoundBufferMgr m_soundBufferMgr;

private:
    bool m_isRunning;

public:
    class Exception : public IException
    {
    public:
        Exception(int line, const char *file, const char *errorString) noexcept;
        const char *what() const noexcept override;
        virtual const char *GetType() const noexcept override;
        const char *GetErrorString() const noexcept;

    private:
        std::string errorString;
    };
};