#pragma once

#include "Saffron/Base.h"
#include "Saffron/Core/Filesystem.h"
#include "Saffron/Core/Window.h"
#include "Saffron/Core/Run.h"
#include "Saffron/Graphics/RenderTargetManager.h"
#include "Saffron/Gui/Gui.h"
#include "Saffron/Gui/FadePane.h"
#include "Saffron/Gui/MenuBar.h"
#include "Saffron/Gui/SplashScreenPane.h"
#include "Saffron/Input/Keyboard.h"
#include "Saffron/Input/Mouse.h"
#include "Saffron/Layer/LayerStack.h"

#include "Saffron/Resource/ComputeShaderStore.h"
#include "Saffron/Resource/FontStore.h"
#include "Saffron/Resource/ImageStore.h"
#include "Saffron/Resource/MusicStore.h"
#include "Saffron/Resource/ShaderStore.h"
#include "Saffron/Resource/SoundStore.h"
#include "Saffron/Resource/SoundBufferStore.h"
#include "Saffron/Resource/TextureStore.h"

namespace Se
{
class Scene;

class AppProperties
{
public:
	std::string Name;
	uint WindowWidth, WindowHeight;
	uint PositionX, PositionY;
	bool Fullscreen;

	static AppProperties CreateFullscreen(std::string name);
	static AppProperties CreateMaximized(std::string name);
	static AppProperties CreateCentered(std::string name, uint windowWidth, uint windowHeight);
};

class App : public Singleton<App>
{
	friend class ApplicationSerializer;

public:
	explicit App(const AppProperties& properties = {"Unnamed", 1280, 720});
	virtual ~App();

	void Run();
	void Exit();

	virtual void OnInit()
	{
	}

	virtual void OnShutdown()
	{
	}

	virtual void OnUpdate();

	virtual void OnGuiRender();

	void PushLayer(std::shared_ptr<Layer> layer);
	void PushOverlay(std::shared_ptr<Layer> overlay);
	void PopLayer(int count = 1);
	void PopOverlay(int count = 1);
	void EraseLayer(std::shared_ptr<Layer> layer);
	void EraseOverlay(std::shared_ptr<Layer> overlay);

	auto Window() const -> Window&;

	static auto ConfigurationName() -> std::string;
	static auto PlatformName() -> std::string;
	static auto Name() -> std::string;

private:
	void OnRenderMenuBar();
	auto OnWindowClose() -> bool;

	void RunFrame();
	void RunLayerFrame();
	void RunSplashScreen();

protected:
	std::shared_ptr<Batch> _splashScreenBatch;

private:
	std::unique_ptr<class Window> _window;
	std::unique_ptr<MenuBar> _menuBar;	
	std::unique_ptr<Filesystem> _filesystem;
	std::unique_ptr<Gui> _gui;

	std::unique_ptr<Keyboard> _keyboard;
	std::unique_ptr<Mouse> _mouse;

	std::unique_ptr<RenderTargetManager> _renderTargetManager;
	std::unique_ptr<class Run> _run;

	std::string _name;

	// Stores
	std::unique_ptr<ComputeShaderStore> _computeShaderStore;
	std::unique_ptr<FontStore> _fontStore;
	std::unique_ptr<ImageStore> _imageStore;
	std::unique_ptr<MusicStore> _musicStore;
	std::unique_ptr<ShaderStore> _shaderStore;
	std::unique_ptr<SoundStore> _soundStore;
	std::unique_ptr<SoundBufferStore> _soundBufferStore;
	std::unique_ptr<TextureStore> _textureStore;

	SplashScreenPane _splashScreenPane;
	bool _running = true, _minimized = false;
	LayerStack _layerStack;

	sf::Time _fpsTimer = sf::Time::Zero;
	int _cachedFps = 0;
	sf::Time _cachedSpf = sf::Time::Zero;
	sf::Time _storedFrametime = sf::Time::Zero;
	int _storedFrameCount = 0;

	FadePane _fadeIn;
};

// Implemented by client
auto CreateApplication() -> std::unique_ptr<App>;
}
