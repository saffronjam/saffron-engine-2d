#include "SaffronPCH.h"

#include "Saffron/Core/App.h"
#include "Saffron/Core/FileIOManager.h"
#include "Saffron/Core/Global.h"
#include "Saffron/Core/Run.h"
#include "Saffron/Input/Keyboard.h"
#include "Saffron/Input/Mouse.h"
#include "Saffron/Gui/Gui.h"
#include "Saffron/Gui/SplashScreenPane.h"
#include "Saffron/Graphics/Scene.h"
#include "Saffron/Graphics/RenderTargetManager.h"

namespace Se
{
App* App::s_Instance = nullptr;


App::App(const Properties& properties) :
	_preLoader(CreateShared<BatchLoader>("Preloader")),
	_window(properties.Name, properties.WindowWidth, properties.WindowHeight),
	_fadeIn(FadePane::Type::In, sf::seconds(0.5f))
{
	Debug::Assert(!s_Instance, "Application already exist");
	s_Instance = this;

	Log::CoreInfo("--- Saffron 2D Framework ---");
	Log::CoreInfo("Creating application {0}", properties.Name);

	_window.SetEventCallback([this](const sf::Event& event)
	{
		OnEvent(event);
	});

	FileIOManager::Init(_window);
	Gui::Init(Filepath("../../../imgui.ini"));

	_preLoader->Submit([]
	{
		Gui::SetStyle(Gui::Style::Dark);
	}, "Initializing GUI");

	Global::Clock::Restart();
}

App::~App()
{
	Gui::Shutdown();
	Log::CoreInfo("Shutting down");
}

void App::PushLayer(std::shared_ptr<Layer> layer)
{
	_layerStack.PushLayer(layer, _preLoader);
}

void App::PushOverlay(std::shared_ptr<Layer> overlay)
{
	_layerStack.PushOverlay(overlay, _preLoader);
}

void App::PopLayer(int count)
{
	_layerStack.PopLayer(count);
}

void App::PopOverlay(int count)
{
	_layerStack.PopOverlay(count);
}

void App::EraseLayer(std::shared_ptr<Layer> layer)
{
	_layerStack.EraseLayer(layer);
}

void App::EraseOverlay(std::shared_ptr<Layer> overlay)
{
	_layerStack.EraseOverlay(overlay);
}

void App::Run()
{
	OnInit();

	while (_running)
	{
		if (Keyboard::IsDown(sf::Keyboard::Key::A))
		{
			Log::Info("TEST");
		}

		if (!_preLoader->IsFinished())
		{
			RunSplashScreen();
			_fadeIn.Start();
		}

		Global::Clock::Restart();
		_window.HandleBufferedEvents();
		_window.Clear();
		RenderTargetManager::ClearAll();
		if (!_minimized)
		{
			Gui::Begin();
			for (const auto& layer : _layerStack)
			{
				layer->OnPreFrame();
			}
			for (const auto& layer : _layerStack)
			{
				layer->OnUpdate();
			}
			for (const auto& layer : _layerStack)
			{
				layer->OnGuiRender();
			}
			for (const auto& layer : _layerStack)
			{
				layer->OnPostFrame();
			}
			_fadeIn.OnUpdate();
			_fadeIn.OnGuiRender();
			Gui::End();

			Keyboard::OnUpdate();
			Mouse::OnUpdate();
		}
		OnUpdate();
		Run::Execute();
		RenderTargetManager::DisplayAll();
		_window.Display();
	}

	_layerStack.Clear();

	OnShutdown();
}

void App::Exit()
{
	_preLoader->ForceExit();
	_running = false;
}

void App::OnGuiRender()
{
	if (ImGui::Begin("Stats"))
	{
		const auto dt = Global::Clock::GetFrameTime();
		_fpsTimer += dt;
		if (_fpsTimer.asSeconds() < 1.0f)
		{
			_storedFrameCount++;
			_storedFrametime += dt;
		}
		else
		{
			_cachedFps = static_cast<int>(static_cast<float>(_storedFrameCount) / _storedFrametime.asSeconds());
			_cachedSpf = sf::seconds(_storedFrametime.asSeconds() / static_cast<float>(_storedFrameCount));
			_storedFrameCount = 0;
			_storedFrametime = sf::Time::Zero;
			_fpsTimer = sf::Time::Zero;
		}

		Gui::BeginPropertyGrid();

		Gui::Property("Vendor", "SFML v.2.5.2");
		Gui::Property("Frametime", std::to_string(_cachedSpf.asMicroseconds() / 1000.0f) + " ms");
		Gui::Property("FPS", std::to_string(_cachedFps));

		Gui::EndPropertyGrid();
	}
	ImGui::End();
}

void App::OnEvent(const sf::Event& event)
{
	if (event.type == event.Closed)
	{
		OnWindowClose();
	}

	Gui::OnEvent(event);
	Keyboard::OnEvent(event);
	Mouse::OnEvent(event);

	for (auto it = _layerStack.end(); it != _layerStack.begin();)
	{
		(*--it)->OnEvent(event);
	}
}

bool App::OnWindowClose()
{
	Exit();
	return true;
}

void App::RunSplashScreen()
{
	_preLoader->Execute();

	SplashScreenPane splashScreenPane(_preLoader);
	while (!splashScreenPane.IsFinished())
	{
		_window.Clear();
		RenderTargetManager::ClearAll();
		Gui::Begin();
		splashScreenPane.OnUpdate();
		splashScreenPane.OnGuiRender();
		_window.HandleBufferedEvents();
		Gui::End();
		Run::Execute();
		RenderTargetManager::DisplayAll();
		_window.Display();
		Global::Clock::Restart();
		const auto step = Global::Clock::GetFrameTime().asSeconds();
		const auto duration = splashScreenPane.GetBatchLoader()->IsFinished()
			                      ? 0ll
			                      : std::max(0ll, static_cast<long long>(1000.0 / 60.0 - step));
		std::this_thread::sleep_for(std::chrono::milliseconds(duration));
	}
}

String App::GetConfigurationName()
{
#if defined(SE_DEBUG)
	return "Debug";
#elif defined(SE_RELEASE)
	return "Release";
#elif defined(SE_DIST)
	return "Dist";
#else
#error Undefined configuration?
#endif
}

String App::GetPlatformName()
{
#if defined(SE_PLATFORM_WINDOWS)
	return "Windows x64";
#elif defined(SE_PLATFORM_LINUX)
	return "Linux x64";
#endif
}
}
