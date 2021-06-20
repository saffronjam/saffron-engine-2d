#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Saffron/Base.h"
#include "Saffron/Core/Enums.h"
#include "Saffron/Libraries/GenUtils.h"
#include "Saffron/Interface/IException.h"

#ifdef DrawText
#undef DrawText
#define HadDrawText
#endif

namespace Se
{
class Window
{
	friend class Camera;

public:
	Window(String title, int width, int height);
	~Window() = default;
	Window(const Window&) = delete;
	auto operator=(const Window&) -> Window& = delete;

	void Draw(const sf::Drawable& drawable, sf::RenderStates renderStates = sf::RenderStates::Default);
	void DrawText(const sf::Text& text, TextAlign align = TextAlign::Left,
	              sf::RenderStates renderStates = sf::RenderStates::Default);
	void DrawPoint(const sf::Vector2f& position, sf::Color color = sf::Color::Red, float radius = 3.0f);
	void DrawRect(const sf::FloatRect& rect, sf::Color fillColor = sf::Color::Red, bool outlined = false,
	              sf::Color outlineColor = sf::Color::Black);
	void DrawLine(const sf::Vector2f& first, const sf::Vector2f& second, sf::Color color = sf::Color::Red);

	void Clear();
	void Display();

	void HandleBufferedEvents();

	void PositionCenter();

	auto NativeWindow() -> sf::RenderWindow&;
	auto NativeWindow() const -> const sf::RenderWindow&;

	auto Position() const -> sf::Vector2i;
	auto Size() const -> sf::Vector2u;
	auto Width() const -> int;
	auto Height() const -> int;
	auto Title() const -> const String&;
	auto ScreenRect() const -> sf::IntRect;

	auto IsFullscreen() const -> bool { return _fullscreen; }

	void SetPosition(const sf::Vector2i& pos);
	void Resize(const sf::Vector2u& size);
	void SetTitle(const String& title);
	void SetIcon(const Path& path);
	void SetFullscreen(bool toggle);
	void SetVSync(bool toggle);

public:
	EventSubscriberList<const sf::Event&> AnyEvent;

	// Window events
	EventSubscriberList<void> Closed;
	EventSubscriberList<const sf::Event::SizeEvent&> Resized;
	EventSubscriberList<void> LostFocus;
	EventSubscriberList<void> GainedFocus;

	// Keyboard events
	EventSubscriberList<const sf::Event::TextEvent&> TextEntered;
	EventSubscriberList<const sf::Event::KeyEvent&> KeyPressed;
	EventSubscriberList<const sf::Event::KeyEvent&> KeyReleased;

	// Mouse events
	EventSubscriberList<const sf::Event::MouseWheelScrollEvent&> MouseWheelScrolled;
	EventSubscriberList<const sf::Event::MouseButtonEvent&> MouseButtonPressed;
	EventSubscriberList<const sf::Event::MouseButtonEvent&> MouseButtonReleased;
	EventSubscriberList<const sf::Event::MouseMoveEvent&> MouseMoved;
	EventSubscriberList<void> MouseEntered;
	EventSubscriberList<void> MouseLeft;

private:
	void Render(const sf::Drawable& drawable, sf::RenderStates renderStates = sf::RenderStates::Default);

private:
	// Used after exiting fullscreen
	sf::VideoMode _videomode;
	String _title;
	sf::Uint32 _style;
	sf::RenderWindow _nativeWindow;
	// Used after exiting fullscreen
	sf::Vector2i _nonFullscreenPosition;

	bool _fullscreen;
};
}

#if defined(HadDrawText)
#define DrawText DrawTextA
#undef HadDrawText
#endif
