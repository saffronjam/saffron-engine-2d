#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "Saffron/Base.h"

namespace Se
{
class Keyboard : public Singleton<Keyboard>
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
	SubscriberList<const sf::Event::KeyEvent &> Pressed;
	SubscriberList<const sf::Event::KeyEvent &> Released;
	SubscriberList<const sf::Event::TextEvent> TextEntered;

private:
	std::unordered_map<sf::Keyboard::Key, bool> _keymap;
	std::unordered_map<sf::Keyboard::Key, bool> _prevKeymap;
	std::u32string _textInputBuffer;
};
}
