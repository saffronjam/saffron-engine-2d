#include "SaffronPCH.h"

#include "Saffron/Core/App.h"
#include "Saffron/Input/Keyboard.h"

namespace Se
{
Keyboard::Keyboard() :
	SingleTon(this)
{
	auto& win = App::Instance().Window();

	win.KeyPressed += SE_EV_ACTION(Keyboard::OnPress);
	win.KeyReleased += SE_EV_ACTION(Keyboard::OnRelease);
	win.TextEntered += SE_EV_ACTION(Keyboard::OnTextInput);
}

void Keyboard::OnUpdate()
{
	auto& inst = Instance();

	for (auto& [key, state] : inst._keymap)
	{
		inst._prevKeymap[key] = state;
	}

	inst._textInputBuffer.clear();
}

auto Keyboard::IsDown(const sf::Keyboard::Key& key) -> bool
{
	auto& inst = Instance();

	return inst._keymap[key];
}

auto Keyboard::IsPressed(const sf::Keyboard::Key& key) -> bool
{
	auto& inst = Instance();

	return inst._keymap[key] && !inst._prevKeymap[key];
}

auto Keyboard::IsReleased(const sf::Keyboard::Key& key) -> bool
{
	auto& inst = Instance();

	return !inst._keymap[key] && inst._prevKeymap[key];
}

auto Keyboard::IsAnyDown() -> bool
{
	auto& inst = Instance();

	for (const auto& key : inst._keymap | std::views::keys)
	{
		if (inst._keymap[key])
		{
			return true;
		}
	}
	return false;
}

auto Keyboard::TextInput() -> std::u32string
{
	auto& inst = Instance();

	return inst._textInputBuffer;
}

void Keyboard::OnPress(const sf::Event::KeyEvent& event)
{
	auto& inst = Instance();

	inst._keymap[event.code] = true;
	inst.Pressed.Invoke(event);
}

void Keyboard::OnRelease(const sf::Event::KeyEvent& event)
{
	auto& inst = Instance();

	inst._keymap[event.code] = false;
	inst.Released.Invoke(event);
}

void Keyboard::OnTextInput(const sf::Event::TextEvent& event)
{
	auto& inst = Instance();

	inst._textInputBuffer += event.unicode;
	inst.TextEntered.Invoke(event);
}
}
