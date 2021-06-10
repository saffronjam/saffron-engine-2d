#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Saffron/Base.h"

namespace Se
{
class Keyboard : public SingleTon<Keyboard>
{
public:
	Keyboard();

	void OnUpdate();

	static auto IsDown(const sf::Keyboard::Key& key) -> bool;
	static auto IsPressed(const sf::Keyboard::Key& key) -> bool;
	static auto IsReleased(const sf::Keyboard::Key& key) -> bool;
	static auto IsAnyDown() -> bool;

	static auto TextInput() -> std::u32string;

private:
	static void OnPress(const sf::Event::KeyEvent& event);
	static void OnRelease(const sf::Event::KeyEvent& event);
	static void OnTextInput(const sf::Event::TextEvent& event);

public:
	EventSubscriberList<const sf::Event::KeyEvent &> Pressed;
	EventSubscriberList<const sf::Event::KeyEvent &> Released;
	EventSubscriberList<const sf::Event::TextEvent> TextEntered;

private:
	HashMap<sf::Keyboard::Key, bool> _keymap;
	HashMap<sf::Keyboard::Key, bool> _prevKeymap;
	std::u32string _textInputBuffer;
};
}
