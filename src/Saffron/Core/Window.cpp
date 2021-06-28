#include "SaffronPCH.h"

#include <glad/glad.h>

#include <SFML/Graphics/CircleShape.hpp>

#include "Saffron/Core/Window.h"
#include "Saffron/Lighting/LightningMgr.h"
#include "Saffron/Resource/ImageStore.h"

#include <SFML/System/Err.hpp>

#ifdef DrawText
#undef DrawText
#endif

namespace Se
{
Window::Window(String title, int width, int height) :
	_videomode(width, height),
	_title(Move(title)),
	_style(sf::Style::Resize | sf::Style::Titlebar | sf::Style::Close),
	_nativeWindow(sf::RenderWindow(_videomode, Move(title), _style, sf::ContextSettings(0u, 0u, 0u, 1u, 4u, 0u)))
{
	_nativeWindow.setKeyRepeatEnabled(false);
	_nativeWindow.resetGLStates();
	SetTitle(_title);
	PositionCenter();

	SetVSync(true);

	if (!gladLoadGL())
	{
		Debug::Break("Failed to initialize OpenGL context");
	}

	sf::err().rdbuf(_sfmlStreamBuffer.rdbuf());
}

void Window::Draw(const sf::Drawable& drawable, sf::RenderStates renderStates)
{
	Render(drawable, renderStates);
}

void Window::DrawText(const sf::Text& text, TextAlign align, sf::RenderStates renderStates)
{
	auto textCpy = text;
	sf::Vector2f offset(0.0f, 0.0f);
	switch (align)
	{
	case TextAlign::Left: break;
	case TextAlign::Middle: offset.x = textCpy.getLocalBounds().width / 2.0f;
		break;
	case TextAlign::Right: offset.x = textCpy.getLocalBounds().width;
		break;
	}

	textCpy.setPosition(text.getPosition() + offset);
	Render(textCpy, renderStates);
}

void Window::DrawPoint(const sf::Vector2f& position, sf::Color color, float radius)
{
	sf::CircleShape circle(radius);
	circle.setPosition(position - sf::Vector2f(radius, radius));
	circle.setFillColor(color);
	Draw(circle);
}

void Window::DrawRect(const sf::FloatRect& rect, sf::Color fillColor, bool outlined, sf::Color outlineColor)
{
	sf::RectangleShape rectShape;
	rectShape.setPosition(rect.left, rect.top);
	rectShape.setSize(sf::Vector2f(rect.width, rect.height));
	rectShape.setFillColor(fillColor);
	if (outlined)
	{
		rectShape.setOutlineThickness(1);
		rectShape.setOutlineColor(outlineColor);
	}
	Draw(rectShape);
}

void Window::DrawLine(const sf::Vector2f& first, const sf::Vector2f& second, sf::Color color)
{
	sf::VertexArray line(sf::PrimitiveType::Lines, 2);
	line[0].color = color;
	line[1].color = color;
	line[0].position = first;
	line[1].position = second;
	Draw(line);
}

void Window::Clear()
{
	_nativeWindow.clear();
}

void Window::Display()
{
	_nativeWindow.display();
}

void Window::HandleBufferedEvents()
{
	sf::Event event;
	while (_nativeWindow.pollEvent(event))
	{
		switch (event.type)
		{
#define CASE(eventType, arg)  case sf::Event:: ## eventType: { (eventType).Invoke(arg); break; }

			// Window events
		CASE(Closed,);
		CASE(Resized, event.size);
		CASE(LostFocus,);
		CASE(GainedFocus,);

			// Keyboard events
		CASE(TextEntered, event.text);
		CASE(KeyPressed, event.key);
		CASE(KeyReleased, event.key);

			// Mouse events
		CASE(MouseWheelScrolled, event.mouseWheelScroll);
		CASE(MouseButtonPressed, event.mouseButton);
		CASE(MouseButtonReleased, event.mouseButton);
		CASE(MouseMoved, event.mouseMove);
		CASE(MouseEntered,);
		CASE(MouseLeft,);

#undef CASE

			// Unhandled at the time
		case sf::Event::MouseWheelMoved:
		case sf::Event::JoystickButtonPressed:
		case sf::Event::JoystickButtonReleased:
		case sf::Event::JoystickMoved:
		case sf::Event::JoystickConnected:
		case sf::Event::JoystickDisconnected:
		case sf::Event::TouchBegan:
		case sf::Event::TouchMoved:
		case sf::Event::TouchEnded:
		case sf::Event::SensorChanged:
		case sf::Event::Count: default:
		{
			// Avoid "AnyEvent" invoke
			continue;
		}
		}
		AnyEvent.Invoke(event);
	}
}

void Window::HandleBufferedMessages()
{
	if (!_sfmlStreamBuffer.view().empty())
	{
		Log::CoreInfo("{0}[SFML Log]{1} {2}", _sfmlLogFmt, Log::Fmt::Reset,_sfmlStreamBuffer.view());
		_sfmlStreamBuffer = OStringStream();
		sf::err().rdbuf(_sfmlStreamBuffer.rdbuf());
	}
}

void Window::PositionCenter()
{
	const sf::VideoMode max = sf::VideoMode::getDesktopMode();
	const sf::Vector2i halfSize = sf::Vector2i(_nativeWindow.getSize().x / 2u, _nativeWindow.getSize().y / 2u);
	_nativeWindow.setPosition(sf::Vector2i(max.width, max.height) / 2 - halfSize);
}

auto Window::NativeWindow() -> sf::RenderWindow&
{
	return _nativeWindow;
}

auto Window::NativeWindow() const -> const sf::RenderWindow&
{
	return _nativeWindow;
}

auto Window::Position() const -> sf::Vector2i
{
	return _nativeWindow.getPosition();
}

auto Window::Size() const -> sf::Vector2u
{
	return _nativeWindow.getSize();
}

auto Window::Width() const -> int
{
	return Size().x;
}

auto Window::Height() const -> int
{
	return Size().y;
}

auto Window::Title() const -> const std::string&
{
	return _title;
}

auto Window::ScreenRect() const -> sf::IntRect
{
	return sf::IntRect(0, 0, Width(), Height());
}

void Window::SetPosition(const sf::Vector2i& pos)
{
	_nativeWindow.setPosition(pos);
}

void Window::Resize(const sf::Vector2u& size)
{
	_nativeWindow.setSize(size);
}

void Window::SetTitle(const String& title)
{
	_nativeWindow.setTitle(title);
}

void Window::SetIcon(const Path& path)
{
	_icon = ImageStore::Get(path, false);
	RefreshIcon();
}

void Window::SetFullscreen(bool toggle)
{
	if (toggle && !_fullscreen)
	{
		_fullscreen = true;
		_videomode.width = Size().x;
		_videomode.height = Size().y;
		_nonFullscreenPosition = Position();
		_nativeWindow.create(sf::VideoMode::getFullscreenModes()[0], Title(), sf::Style::Fullscreen);
		RefreshIcon();
	}
	else if (!toggle && _fullscreen)
	{
		_fullscreen = false;
		_nativeWindow.create(_videomode, Title(), _style);
		SetPosition(_nonFullscreenPosition);
		RefreshIcon();
	}
}

void Window::SetVSync(bool toggle)
{
	_nativeWindow.setVerticalSyncEnabled(toggle);
}

void Window::Render(const sf::Drawable& drawable, sf::RenderStates renderStates)
{
	_nativeWindow.draw(drawable, renderStates);
}

void Window::RefreshIcon()
{
	if(_icon.has_value())
	{
		const auto shrdIcon = _icon.value();
		_nativeWindow.setIcon(shrdIcon->getSize().x, shrdIcon->getSize().y, shrdIcon->getPixelsPtr());		
	}
}
}
