#pragma once

#include <map>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace Se
{
class Keyboard
{
public:
	Keyboard() = default;
	~Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	auto operator=(const Keyboard&) -> Keyboard& = delete;

	static void OnUpdate();
	static void OnEvent(const sf::Event& event);

	static auto IsDown(const sf::Keyboard::Key& key) -> bool;
	static auto IsPressed(const sf::Keyboard::Key& key) -> bool;
	static auto IsReleased(const sf::Keyboard::Key& key) -> bool;
	static auto IsAnyDown() -> bool;

	static auto GetTextInput() -> std::string { return _textInputBuffer; }

private:

	static void OnPress(const sf::Event::KeyEvent& event);
	static void OnRelease(const sf::Event::KeyEvent& event);
	static void OnTextInput(unsigned char character);

private:
	static std::map<sf::Keyboard::Key, bool> _keymap;
	static std::map<sf::Keyboard::Key, bool> _prevKeymap;
	static std::string _textInputBuffer;
};
}
