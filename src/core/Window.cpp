#include "Window.h"

SDL_Window *Window::m_sdlWindow = nullptr;
SDL_Renderer *Window::m_sdlRenderer = nullptr;

Window::~Window()
{
    SDL_DestroyRenderer(m_sdlRenderer);
    SDL_DestroyWindow(m_sdlWindow);
}

void Window::Create(const std::string &title, int width, int height)
{
    if (!SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl"))
    {
        VEWND_LAST_EXCEPT();
    }

    m_sdlWindow = SDL_CreateWindow(title.c_str(),
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   800, 600,
                                   SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!m_sdlWindow)
        VEWND_LAST_EXCEPT();

    m_sdlRenderer = SDL_CreateRenderer(Window::m_sdlWindow,
                                       -1,
                                       SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_sdlRenderer)
        VEWND_LAST_EXCEPT();

    if (SDL_SetRenderDrawBlendMode(m_sdlRenderer, SDL_BLENDMODE_BLEND) == -1 ||
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) == -1 ||
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) == -1 ||
        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1) == -1)
    {
        VEWND_LAST_EXCEPT();
    }
}

void Window::Clear()
{
    assert("Attempted to handle the window without creating it" && m_sdlWindow && m_sdlRenderer);
    if (SDL_RenderClear(m_sdlRenderer) == -1)
    {
        VEWND_LAST_EXCEPT();
    }
}

void Window::Present() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sdlWindow && m_sdlRenderer);
    SDL_RenderPresent(m_sdlRenderer);
}

void Window::SetPosition(int x, int y) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sdlWindow && m_sdlRenderer);
    SDL_SetWindowPosition(m_sdlWindow, x, y);
}

void Window::SetSize(int width, int height) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sdlWindow && m_sdlRenderer);
    SDL_SetWindowSize(m_sdlWindow, width, height);
}

void Window::SetTitle(const std::string &title) noexcept
{
    assert("Attempted to handle the window without creating it" && m_sdlWindow && m_sdlRenderer);
    SDL_SetWindowTitle(m_sdlWindow, title.c_str());
}

void Window::MoveToCenter() noexcept
{
    assert("Attempted to handle the window without creating it" && m_sdlWindow && m_sdlRenderer);
    Window::SetPosition(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
}

Window::Exception::Exception(int line, const char *file, const char *errorString) noexcept
    : VeException(line, file),
      errorString(errorString)
{
}

const char *Window::Exception::what() const noexcept
{
    std::ostringstream oss;
    oss << "[Type] " << GetType() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBuffer = oss.str();
    return whatBuffer.c_str();
}

const char *Window::Exception::GetType() const noexcept
{
    return "V-2DEngine Window Exception";
}

const char *Window::Exception::GetErrorString() const noexcept
{
    return errorString.c_str();
}