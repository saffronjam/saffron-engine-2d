#pragma once

#include <string>
#include <cassert>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_hints.h>

#include "WindowThrowMacros.h"
#include "VeException.h"
#include "Vec2.h"

class Window
{
public:
    Window() = default;
    ~Window();
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    void Create(const std::string &title, int width, int height);

    static void Clear();
    static void Present() noexcept;

    static Vec2i GetPosition() noexcept;
    static int GetWidth() noexcept;
    static int GetHeight() noexcept;
    static const std::string &GetTitle() noexcept;

    static bool IsFullscreen() noexcept;
    static bool IsVSyncEnabled() noexcept;

    static void SetPosition(int x, int y) noexcept;
    static void SetSize(int width, int height) noexcept;
    static void SetTitle(const std::string &title) noexcept;
    static void SetIcon(const std::string &icon) noexcept;
    static void SetFullscreen(bool toggle) noexcept;
    static void SetVSync(bool toggle) noexcept;

    static void MoveToCenter() noexcept;

private:
    static SDL_Window *m_sdlWindow;
    static SDL_Renderer *m_sdlRenderer;

public:
    class Exception : public VeException
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