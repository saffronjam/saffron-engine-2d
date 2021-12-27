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
	Window(std::string title, int width, int height);
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
	void HandleBufferedMessages();

	void PositionCenter();

	auto NativeWindow() -> sf::RenderWindow&;
	auto NativeWindow() const -> const sf::RenderWindow&;

	auto Position() const -> sf::Vector2i;
	auto Size() const -> sf::Vector2u;
	auto Width() const -> int;
	auto Height() const -> int;
	auto Title() const -> const std::string&;
	auto ScreenRect() const -> sf::IntRect;

	auto IsFullscreen() const -> bool { return _fullscreen; }

	void SetPosition(const sf::Vector2i& pos);
	void Resize(const sf::Vector2u& size);
	void SetTitle(const std::string& title);
	void SetIcon(const std::filesystem::path& path);
	void SetFullscreen(bool toggle);
	void SetVSync(bool toggle);

public:
	SubscriberList<const sf::Event&> AnyEvent;

	// Window events
	SubscriberList<void> Closed;
	SubscriberList<const sf::Event::SizeEvent&> Resized;
	SubscriberList<void> LostFocus;
	SubscriberList<void> GainedFocus;

	// Keyboard events
	SubscriberList<const sf::Event::TextEvent&> TextEntered;
	SubscriberList<const sf::Event::KeyEvent&> KeyPressed;
	SubscriberList<const sf::Event::KeyEvent&> KeyReleased;

	// Mouse events
	SubscriberList<const sf::Event::MouseWheelScrollEvent&> MouseWheelScrolled;
	SubscriberList<const sf::Event::MouseButtonEvent&> MouseButtonPressed;
	SubscriberList<const sf::Event::MouseButtonEvent&> MouseButtonReleased;
	SubscriberList<const sf::Event::MouseMoveEvent&> MouseMoved;
	SubscriberList<void> MouseEntered;
	SubscriberList<void> MouseLeft;

private:
	void Render(const sf::Drawable& drawable, sf::RenderStates renderStates = sf::RenderStates::Default);
	void RefreshIcon();

private:
	// Used after exiting fullscreen
	sf::VideoMode _videomode;
	std::string _title;
	sf::Uint32 _style;
	sf::RenderWindow _nativeWindow;
	// Used after exiting fullscreen
	sf::Vector2i _nonFullscreenPosition;

	std::optional<std::shared_ptr<sf::Image>> _icon;

	bool _fullscreen = false;

	std::ostringstream _sfmlStreamBuffer;
	inline static const std::string _sfmlLogFmt = Log::Fmt::OnGreen + Log::Fmt::White;
};
}

#if defined(HadDrawText)
#define DrawText DrawTextA
#undef HadDrawText
#endif
