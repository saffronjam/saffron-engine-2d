#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Saffron/Base.h"

namespace Se
{
class Mouse : public Singleton<Mouse>
{
public:
	Mouse();

	// Move buttonMap into prev-buttonMap
	void OnUpdate();

	static auto IsDown(const sf::Mouse::Button& button) -> bool;
	static auto IsPressed(const sf::Mouse::Button& button) -> bool;
	static auto IsReleased(const sf::Mouse::Button& button) -> bool;
	static auto AnyButtonDown() -> bool;

	static auto CursorPosition(bool normalized = false) -> const sf::Vector2f&;
	static auto Swipe() -> const sf::Vector2f&;

	static auto VerticalScroll() -> float;
	static auto HorizontalScroll() -> float;

private:
	static void OnPress(const sf::Event::MouseButtonEvent& event);
	static void OnRelease(const sf::Event::MouseButtonEvent& event);
	static void OnMove(const sf::Event::MouseMoveEvent& event);
	static void OnEnter();
	static void OnLeave();
	static void OnScroll(const sf::Event::MouseWheelScrollEvent& event);

public:
	SubscriberList<const sf::Event::MouseWheelScrollEvent&> Scrolled;
	SubscriberList<const sf::Event::MouseButtonEvent&> Pressed;
	SubscriberList<const sf::Event::MouseButtonEvent&> Released;
	SubscriberList<const sf::Event::MouseMoveEvent&> Moved;
	SubscriberList<void> Entered;
	SubscriberList<void> Left;

private:
	std::unordered_map<sf::Mouse::Button, bool> _buttonmap;
	std::unordered_map<sf::Mouse::Button, bool> _prevButtonmap;

	sf::Vector2f _mousePosition = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f _mousePositionNDC = sf::Vector2f(0.0f, 0.0f);
	bool _inWindow = true;

	sf::Vector2f _mouseSwipe = sf::Vector2f(0.0f, 0.0f);

	float _verticalScrollBuffer = 0.0f;
	float _horizontalScrollBuffer = 0.0f;
};
}
