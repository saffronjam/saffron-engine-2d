#pragma once

#include "saffron/base.h"
#include "saffron/core/filesystem.h"
#include "saffron/core/window.h"
#include "saffron/core/run.h"
#include "saffron/graphics/render_target_manager.h"
#include "saffron/gui/gui.h"
#include "saffron/gui/fade_pane.h"
#include "saffron/gui/menu_bar.h"
#include "saffron/gui/splash_screen_pane.h"
#include "saffron/input/keyboard.h"
#include "saffron/input/mouse.h"
#include "saffron/layer/layer_stack.h"

#include "saffron/resource/compute_shader_store.h"
#include "saffron/resource/font_store.h"
#include "saffron/resource/image_store.h"
#include "saffron/resource/music_store.h"
#include "saffron/resource/shader_store.h"
#include "saffron/resource/sound_store.h"
#include "saffron/resource/sound_buffer_store.h"
#include "saffron/resource/texture_store.h"

namespace saffron
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
	explicit App(const AppProperties& properties);
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
	void PopLayer(int count);
	void PopOverlay(int count);
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
