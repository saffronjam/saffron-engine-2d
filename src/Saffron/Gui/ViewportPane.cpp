#include "SaffronPCH.h"

#include "Saffron/Gui/ViewportPane.h"
#include "Saffron/Gui/Gui.h"
#include "Saffron/Input/Mouse.h"
#include "Saffron/Libraries/GenUtils.h"

namespace Se
{
ViewportPane::ViewportPane(String windowTitle, const ControllableRenderTexture& target) :
	_windowTitle(Move(windowTitle)),
	_target(&target),
	//_fallbackTexture(Factory::Create<Texture2D>("Resources/Assets/Editor/FallbackViewportPaneTexture.png")),
	_topLeft(0.0f, 0.0f),
	_bottomRight(100.0f, 100.0f),
	_hovered(false),
	_focused(false)
{
}

void ViewportPane::OnGuiRender(bool* open, UUID uuid)
{
	const auto& tl = TopLeft();
	const auto& br = BottomRight();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	OStringStream oss;
	oss << _windowTitle << "##" << uuid;

	ImGui::Begin(oss.str().c_str(), open);

	_dockID = ImGui::GetWindowDockID();

	/*if ( ImGui::IsWindowDocked() )
	{
		auto *wnd = ImGui::FindWindowByName(_windowTitle.c_str());
		if ( wnd )
		{
			ImGuiDockNode *node = wnd->DockNode;
			if ( node && !node->IsHiddenTabBar() )
			{
				node->WantHiddenTabBarToggle = true;
			}
		}
	}*/

	_hovered = ImGui::IsWindowHovered();
	_focused = ImGui::IsWindowFocused();

	const auto viewportOffset = ImGui::GetCursorPos(); // includes tab bar
	ImVec2 minBound = ImGui::GetWindowPos();
	minBound.x += viewportOffset.x;
	minBound.y += viewportOffset.y;

	const auto windowSize = ImGui::GetWindowSize();
	const ImVec2 maxBound = {
		minBound.x + windowSize.x - viewportOffset.x, minBound.y + windowSize.y - viewportOffset.y
	};
	_topLeft = {minBound.x, minBound.y};
	_bottomRight = {maxBound.x, maxBound.y};

	const auto viewportSize = ViewportSize();

	if (_target->IsEnabled())
	{
		Gui::Image(*_target, {viewportSize.x, viewportSize.y}, {0.0f, 1.0f}, {1.0f, 0.0f});
	}
	else
	{
		Gui::Image(_fallbackTexture, {viewportSize.x, viewportSize.y}, sf::FloatRect{0.0f, 0.0f, 1.0f, 1.0f});
	}

	ImGui::GetWindowDrawList()->AddRect(ImVec2(_topLeft.x, tl.y), ImVec2(br.x, br.y),
	                                    _focused ? IM_COL32(255, 140, 0, 180) : IM_COL32(255, 140, 0, 80), 0.0f,
	                                    ImDrawCornerFlags_All, 4);

	Rendered.Invoke();

	ImGui::End();
	ImGui::PopStyleVar();

	if (GenUtils::ConvertTo<unsigned int>(viewportSize) != _target->RenderTexture().getSize())
	{
		Resized.Invoke(viewportSize);
	}
}

auto ViewportPane::InViewport(sf::Vector2f positionNDC) const -> bool
{
	positionNDC.x -= _topLeft.x;
	positionNDC.y -= _topLeft.y;
	return positionNDC.x < _bottomRight.x && positionNDC.y < _bottomRight.y;
}

auto ViewportPane::MousePosition(bool normalized) const -> sf::Vector2f
{
	sf::Vector2f position = Mouse::CursorPosition(false);
	position.x -= _topLeft.x;
	position.y -= _topLeft.y;

	if (normalized)
	{
		const auto viewportWidth = _bottomRight.x - _topLeft.x;
		const auto viewportHeight = _bottomRight.y - _topLeft.y;
		return {position.x / viewportWidth * 2.0f - 1.0f, (position.y / viewportHeight * 2.0f - 1.0f) * -1.0f};
	}
	return {position.x, position.y};
}

auto ViewportPane::ViewportSize() const -> sf::Vector2f
{
	return BottomRight() - TopLeft();
}

auto ViewportPane::DockID() const -> Uint32 { return _dockID; }

auto ViewportPane::TopLeft() const -> const sf::Vector2f& { return _topLeft; }

auto ViewportPane::BottomRight() const -> const sf::Vector2f& { return _bottomRight; }

auto ViewportPane::Hovered() const -> bool { return _hovered; }

auto ViewportPane::Focused() const -> bool { return _focused; }

void ViewportPane::SetTarget(const ControllableRenderTexture& target)
{
	_target = &target;
}
}
