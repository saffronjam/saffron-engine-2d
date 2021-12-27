#include "SaffronPCH.h"

#include <imgui.h>

#include "Saffron/Core/App.h"
#include "Saffron/Input/Mouse.h"

namespace Se
{
Mouse::Mouse() :
	Singleton(this)
{
	auto& win = App::Instance().Window();
	win.MouseWheelScrolled += SE_EV_ACTION(Mouse::OnScroll);
	win.MouseButtonPressed += SE_EV_ACTION(Mouse::OnPress);
	win.MouseButtonReleased += SE_EV_ACTION(Mouse::OnRelease);
	win.MouseMoved += SE_EV_ACTION(Mouse::OnMove);
	win.MouseEntered += SE_EV_ACTION(Mouse::OnEnter);
	win.MouseLeft += SE_EV_ACTION(Mouse::OnLeave);
}

void Mouse::OnUpdate()
{
	auto& inst = Instance();

	for (auto& [key, state] : inst._buttonmap)
	{
		inst._prevButtonmap[key] = state;
	}

	inst._horizontalScrollBuffer = 0.0f;
	inst._verticalScrollBuffer = 0.0f;
	inst._mouseSwipe = {0.0f, 0.0f};
}


auto Mouse::IsDown(const sf::Mouse::Button& button) -> bool
{
	auto& inst = Instance();

	return inst._buttonmap[button];
}

auto Mouse::IsPressed(const sf::Mouse::Button& button) -> bool
{
	auto& inst = Instance();

	return inst._buttonmap[button] && !inst._prevButtonmap[button];
}

auto Mouse::IsReleased(const sf::Mouse::Button& button) -> bool
{
	auto& inst = Instance();

	return !inst._buttonmap[button] && inst._prevButtonmap[button];
}

auto Mouse::AnyButtonDown() -> bool
{
	auto& inst = Instance();
	
	for (auto& state : inst._buttonmap | std::views::values)
	{
		if (state)
		{
			return true;
		}
	}

	return false;
}

auto Mouse::CursorPosition(bool normalized) -> const sf::Vector2f&
{
	auto& inst = Instance();

	if (!normalized)
	{
		return inst._mousePosition;
	}

	const ImVec2 imMousePosition = ImGui::GetMousePos();
	inst._mousePosition.x = imMousePosition.x;
	inst._mousePosition.y = imMousePosition.y;
	return inst._mousePosition;
}

auto Mouse::Swipe() -> const sf::Vector2f&
{
	auto& inst = Instance();

	return inst._mouseSwipe;
}

auto Mouse::VerticalScroll() -> float
{
	auto& inst = Instance();

	return inst._verticalScrollBuffer;
}

auto Mouse::HorizontalScroll() -> float
{
	auto& inst = Instance();

	return inst._horizontalScrollBuffer;
}

void Mouse::OnPress(const sf::Event::MouseButtonEvent& event)
{
	auto& inst = Instance();

	inst._buttonmap[event.button] = true;
	inst.Pressed.Invoke(event);
}

void Mouse::OnRelease(const sf::Event::MouseButtonEvent& event)
{
	auto& inst = Instance();

	inst._buttonmap[event.button] = false;
	inst.Released.Invoke(event);
}

void Mouse::OnMove(const sf::Event::MouseMoveEvent& event)
{
	auto& inst = Instance();

	if (!inst._inWindow && AnyButtonDown() || inst._inWindow)
	{
		const auto oldPosition = inst._mousePosition;
		inst._mousePosition = sf::Vector2f(event.x, event.y);
		inst._mouseSwipe = inst._mousePosition - oldPosition;
	}
	
	inst.Moved.Invoke(event);
}

void Mouse::OnEnter()
{
	auto& inst = Instance();

	inst._inWindow = true;
	inst.Entered.Invoke();
}

void Mouse::OnLeave()
{
	auto& inst = Instance();

	inst._inWindow = false;
	inst.Left.Invoke();
}

void Mouse::OnScroll(const sf::Event::MouseWheelScrollEvent& event)
{
	auto& inst = Instance();

	if (event.wheel == sf::Mouse::Wheel::HorizontalWheel)
	{
		inst._horizontalScrollBuffer += event.delta;
	}
	else if (event.wheel == sf::Mouse::Wheel::VerticalWheel)
	{
		inst._verticalScrollBuffer += event.delta;
	}
	
	inst.Scrolled.Invoke(event);
}
}
