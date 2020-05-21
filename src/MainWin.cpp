#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_mixer.h>

#include "App.h"

void InitSDL();
void QuitSDL();

class InitException;

int main(int, char **)
{
    try
    {
        InitSDL();
        App app;
        app.Run();
    }
    catch (const VeException &e)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, e.GetType(), e.what(), NULL);
    }
    catch (const std::exception &e)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Standard Exception", e.what(), NULL);
    }
    catch (...)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Standard Exception", "No details available", NULL);
    }
    QuitSDL();
    return EXIT_SUCCESS;
}

class InitException : public VeException
{
public:
    InitException(int line, const char *file, const char *errorString) noexcept
        : VeException(line, file),
          errorString(errorString)
    {
    }
    const char *what() const noexcept override
    {
        std::ostringstream oss;
        oss << "[Type] " << GetType() << std::endl
            << "[Description] " << GetErrorString() << std::endl
            << GetOriginString();
        whatBuffer = oss.str();
        return whatBuffer.c_str();
    }
    virtual const char *GetType() const noexcept override
    {
        return "V-2DEngine Init Exception";
    }
    const char *GetErrorString() const noexcept
    {
        return errorString.c_str();
    }

private:
    std::string errorString;
};

void InitSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        throw InitException(__LINE__, __FILE__, SDL_GetError());
    if (SDLNet_Init() < 0)
        throw InitException(__LINE__, __FILE__, SDLNet_GetError());
    if (TTF_Init() < 0)
        throw InitException(__LINE__, __FILE__, TTF_GetError());
    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) < 0)
        throw InitException(__LINE__, __FILE__, Mix_GetError());
    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0)
        throw InitException(__LINE__, __FILE__, IMG_GetError());
}

void QuitSDL()
{
    IMG_Quit();
    Mix_Quit();
    TTF_Quit();
    SDLNet_Quit();
    SDL_Quit();
}