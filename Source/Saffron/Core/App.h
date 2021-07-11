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
	String Name;
	uint WindowWidth, WindowHeight;
	uint PositionX, PositionY;
	bool Fullscreen;

	static AppProperties CreateFullscreen(String name);
	static AppProperties CreateMaximized(String name);
	static AppProperties CreateCentered(String name, uint windowWidth, uint windowHeight);
};

class App : public SingleTon<App>
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

	static auto ConfigurationName() -> String;
	static auto PlatformName() -> String;
	static auto Name() -> String;

private:
	void OnRenderMenuBar();
	auto OnWindowClose() -> bool;

	void RunFrame();
	void RunLayerFrame();
	void RunSplashScreen();

protected:
	Shared<Batch> _splashScreenBatch;

private:
	Unique<class Window> _window;
	Unique<MenuBar> _menuBar;	
	Unique<Filesystem> _filesystem;
	Unique<Gui> _gui;

	Unique<Keyboard> _keyboard;
	Unique<Mouse> _mouse;

	Unique<RenderTargetManager> _renderTargetManager;
	Unique<class Run> _run;

	String _name;

	// Stores
	Unique<ComputeShaderStore> _computeShaderStore;
	Unique<FontStore> _fontStore;
	Unique<ImageStore> _imageStore;
	Unique<MusicStore> _musicStore;
	Unique<ShaderStore> _shaderStore;
	Unique<SoundStore> _soundStore;
	Unique<SoundBufferStore> _soundBufferStore;
	Unique<TextureStore> _textureStore;

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
auto CreateApplication() -> Unique<App>;
}
