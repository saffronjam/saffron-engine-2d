#include "SaffronPCH.h"

#include "Saffron/Core/App.h"
#include "Saffron/Core/Global.h"
#include "Saffron/Gui/Gui.h"
#include "Saffron/Gui/GuiImpl.h"

#include <imgui_internal.h>

namespace Se
{
static int s_UIContextID = 0;

Gui::Gui(Path iniFilepath) :
	SingleTon(this),
	_iniFilepath(Move(iniFilepath))
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = "../../../imgui.ini"; //reinterpret_cast<const char*>(_iniFilepath.c_str());
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge

	auto* newFont = AddFont("res/Fonts/segoeui.ttf", 18);
	io.FontDefault = newFont;
	_currentFont = {18, newFont};

	AddFont("res/Fonts/segoeui.ttf", 8);
	AddFont("res/Fonts/segoeui.ttf", 12);
	AddFont("res/Fonts/segoeui.ttf", 14);
	AddFont("res/Fonts/segoeui.ttf", 20);
	AddFont("res/Fonts/segoeui.ttf", 22);
	AddFont("res/Fonts/segoeui.ttf", 24);
	AddFont("res/Fonts/segoeui.ttf", 28);
	AddFont("res/Fonts/segoeui.ttf", 32);
	AddFont("res/Fonts/segoeui.ttf", 56);
	AddFont("res/Fonts/segoeui.ttf", 72);

	auto& win = App::Instance().Window();
	GuiImpl::SFML::Init(win.NativeWindow());
	win.AnyEvent += [](const sf::Event& event)
	{
		GuiImpl::SFML::ProcessEvent(event);
		return false;
	};

	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.TabRounding = 0.0f;
	}

	SetStyle(GuiStyle::Dark);
}

Gui::~Gui()
{
	GuiImpl::SFML::Shutdown();
}

void Gui::Begin()
{
	GuiImpl::SFML::Update(App::Instance().Window().NativeWindow(), Global::Clock::FrameTime());
}

void Gui::End()
{
	ImGuiIO& io = ImGui::GetIO();

	GuiImpl::SFML::Render(App::Instance().Window().NativeWindow());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void Gui::BeginPropertyGrid(String id, float width)
{
	PushID();
	ImGui::Columns(2, id.length() ? id.c_str() : nullptr);
	ImGui::AlignTextToFramePadding();
}

void Gui::EndPropertyGrid()
{
	ImGui::Columns(1);
	PopID();
}

auto Gui::BeginTreeNode(const String& name, bool defaultOpen) -> bool
{
	ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
		ImGuiTreeNodeFlags_FramePadding;
	if (defaultOpen) treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

	return ImGui::TreeNodeEx(name.c_str(), treeNodeFlags);
}

void Gui::EndTreeNode()
{
	ImGui::TreePop();
}

void Gui::Property(const String& name, const Function<void()>& onClick, bool secondColumn)
{
	if (secondColumn)
	{
		ImGui::NextColumn();
	}

	const String id = name + "##" + name;
	if (ImGui::Button(id.c_str(), {ImGui::GetContentRegionAvailWidth(), 0}))
	{
		onClick();
	}

	if (!secondColumn)
	{
		ImGui::NextColumn();
	}
	ImGui::NextColumn();
}

void Gui::Property(const String& name, const String& value)
{
	ImGui::Text("%s", name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	const String id = "##" + name;
	ImGui::InputText(id.c_str(), const_cast<char*>(value.c_str()), 256, ImGuiInputTextFlags_ReadOnly);

	ImGui::PopItemWidth();
	ImGui::NextColumn();
}

auto Gui::Property(const String& name, String& value) -> bool
{
	ImGui::Text("%s", name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	char buffer[256];
	strcpy(buffer, value.c_str());

	const String id = "##" + name;
	bool changed = false;
	if (ImGui::InputText(id.c_str(), buffer, 256))
	{
		value = buffer;
		changed = true;
	}
	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return changed;
}

auto Gui::Property(const String& name, bool& value) -> bool
{
	ImGui::Text("%s", name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	const String id = "##" + name;
	const bool result = ImGui::Checkbox(id.c_str(), &value);

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return result;
}

auto Gui::Property(const String& name, const String& text, const String& buttonName,
                   const Function<void()>& onButtonPress) -> bool
{
	ImGui::Text("%s", name.c_str());
	ImGui::NextColumn();

	const auto minButtonWidth = ImGui::CalcTextSize(buttonName.c_str()).x + 8.0f;
	const auto textBoxWidth = ImGui::GetContentRegionAvailWidth() - minButtonWidth;

	if (textBoxWidth > 0.0f)
	{
		ImGui::PushItemWidth(textBoxWidth);
		char buffer[256];
		strcpy(buffer, text.c_str());
		const String id = "##" + name;
		ImGui::InputText(id.c_str(), const_cast<char*>(text.c_str()), 256, ImGuiInputTextFlags_ReadOnly);
		ImGui::PopItemWidth();
		ImGui::SameLine();
	}

	bool changed = false;
	const auto contentRegionAvailable = ImGui::GetContentRegionAvailWidth();
	if (contentRegionAvailable > 0.0f)
	{
		if (ImGui::Button(buttonName.c_str(), {ImGui::GetContentRegionAvailWidth(), 0.0f}))
		{
			if (onButtonPress) onButtonPress();
			changed = true;
		}
	}

	ImGui::NextColumn();

	return changed;
}

auto Gui::Property(const String& name, int& value, int min, int max, float step, GuiPropertyFlag flags) -> bool
{
	return Property(name, value, "%d", min, max, step, flags);
}

auto Gui::Property(const String& name, int& value, const char* format, int min, int max, float step,
                   GuiPropertyFlag flags) -> bool
{
	ImGui::Text("%s", name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	const auto imGuiFlags = static_cast<ImGuiSliderFlags>(flags);

	const String id = "##" + name;
	bool changed = false;
	if (flags & GuiPropertyFlag_Slider)
	{
		changed = ImGui::SliderInt(id.c_str(), &value, min, max, format, imGuiFlags);
	}
	else if (flags & GuiPropertyFlag_Drag)
	{
		changed = ImGui::DragInt(id.c_str(), &value, step, min, max, format, imGuiFlags);
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return changed;
}

auto Gui::Property(const String& name, float& value, float min, float max, float step, GuiPropertyFlag flags) -> bool
{
	return Property(name, value, "%.3f", min, max, step, flags);
}

auto Gui::Property(const String& name, float& value, const char* format, float min, float max, float step,
                   GuiPropertyFlag flags) -> bool
{
	ImGui::Text("%s", name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	const auto imGuiFlags = static_cast<ImGuiSliderFlags>(flags);

	const String id = "##" + name;
	bool changed = false;
	if (flags & GuiPropertyFlag_Slider)
	{
		changed = ImGui::SliderFloat(id.c_str(), &value, min, max, format, imGuiFlags);
	}
	else if (flags & GuiPropertyFlag_Drag)
	{
		changed = ImGui::DragFloat(id.c_str(), &value, step, min, max, format, imGuiFlags);
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return changed;
}

auto Gui::Property(const String& name, sf::Vector2f& value, GuiPropertyFlag flags) -> bool
{
	return Property(name, value, -1.0f, 1.0f, 1.0f, flags);
}

auto Gui::Property(const String& name, sf::Vector2f& value, float min, float max, float step,
                   GuiPropertyFlag flags) -> bool
{
	return Property(name, value, "%.03f", min, max, step, flags);
}

auto Gui::Property(const String& name, sf::Vector2f& value, const char* format, float min, float max, float step,
                   GuiPropertyFlag flags) -> bool
{
	ImGui::Text("%s", name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	const auto imGuiFlags = static_cast<ImGuiSliderFlags>(flags);

	const String id = "##" + name;
	bool changed = false;
	if (flags & GuiPropertyFlag_Slider)
	{
		changed = ImGui::SliderFloat2(id.c_str(), reinterpret_cast<float*>(&value), min, max, format, imGuiFlags);
	}
	else if (flags & GuiPropertyFlag_Drag)
	{
		changed = ImGui::DragFloat2(id.c_str(), reinterpret_cast<float*>(&value), step, min, max, format, imGuiFlags);
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return changed;
}

auto Gui::Property(const String& name, sf::Vector3f& value, GuiPropertyFlag flags) -> bool
{
	return Property(name, value, -1.0f, 1.0f, 1.0f, flags);
}

auto Gui::Property(const String& name, sf::Vector3f& value, float min, float max, float step, GuiPropertyFlag flags,
                   Optional<std::function<void()>> fn) -> bool
{
	return Property(name, value, "%.3f", min, max, step, flags);
}

auto Gui::Property(const String& name, sf::Vector3f& value, const char* format, float min, float max, float step,
                   GuiPropertyFlag flags, Optional<std::function<void()>> fn) -> bool
{
	ImGui::Text("%s", name.c_str());
	ImGui::NextColumn();

	const auto imGuiFlags = static_cast<ImGuiSliderFlags>(flags);

	if (fn.has_value())
	{
		ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.75f);
	}
	else
	{
		ImGui::PushItemWidth(-1);
	}

	const String id = "##" + name;
	bool changed = false;
	if (flags & GuiPropertyFlag_Color)
	{
		changed = ImGui::ColorEdit3(id.c_str(), reinterpret_cast<float*>(&value), ImGuiColorEditFlags_NoInputs);
	}
	else if (flags & GuiPropertyFlag_Slider)
	{
		changed = ImGui::SliderFloat3(id.c_str(), reinterpret_cast<float*>(&value), min, max, format, imGuiFlags);
	}
	else if (flags & GuiPropertyFlag_Drag)
	{
		changed = ImGui::DragFloat3(id.c_str(), reinterpret_cast<float*>(&value), step, min, max, format, imGuiFlags);
	}

	if (fn.has_value())
	{
		const String buttonID = "<" + id + "##fn";
		ImGui::SameLine();
		if (ImGui::Button(buttonID.c_str(), {ImGui::GetContentRegionAvailWidth(), 0.0f}))
		{
			fn.value()();
			changed = false;
		}
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return changed;
}

auto Gui::Property(const String& name, sf::Vector4f& value, GuiPropertyFlag flags) -> bool
{
	return Property(name, value, -1.0f, 1.0f, 1.0f, flags);
}

auto Gui::Property(const String& name, sf::Vector4f& value, float min, float max, float step,
                   GuiPropertyFlag flags) -> bool
{
	return Property(name, value, "%.3f", min, max, step, flags);
}

auto Gui::Property(const String& name, sf::Vector4f& value, const char* format, float min, float max, float step,
                   GuiPropertyFlag flags) -> bool
{
	ImGui::Text(name.c_str());
	ImGui::NextColumn();
	ImGui::PushItemWidth(-1);

	const auto imGuiFlags = static_cast<ImGuiSliderFlags>(flags);

	const String id = "##" + name;
	bool changed = false;
	if (flags & GuiPropertyFlag_Color)
	{
		changed = ImGui::ColorEdit4(id.c_str(), reinterpret_cast<float*>(&value), ImGuiColorEditFlags_NoInputs);
	}
	else if (flags & GuiPropertyFlag_Slider)
	{
		changed = ImGui::SliderFloat4(id.c_str(), reinterpret_cast<float*>(&value), min, max, "%.3f", imGuiFlags);
	}
	else if (flags & GuiPropertyFlag_Drag)
	{
		changed = ImGui::DragFloat4(id.c_str(), reinterpret_cast<float*>(&value), step, min, max, "%.3f", imGuiFlags);
	}

	ImGui::PopItemWidth();
	ImGui::NextColumn();

	return changed;
}

void Gui::Image(const sf::Texture& texture, const sf::Color& tintColor, const sf::Color& borderColor)
{
	GuiImpl::Image(texture, tintColor, borderColor);
}

void Gui::Image(const sf::Texture& texture, const sf::Vector2f& size, const sf::Color& tintColor,
                const sf::Color& borderColor)
{
	GuiImpl::Image(texture, size, tintColor, borderColor);
}

void Gui::Image(const sf::Texture& texture, const sf::FloatRect& uvRect, const sf::Color& tintColor,
                const sf::Color& borderColor)
{
	GuiImpl::Image(texture, uvRect, tintColor, borderColor);
}

void Gui::Image(const sf::Texture& texture, const sf::Vector2f& size, const sf::FloatRect& uvRect,
                const sf::Color& tintColor, const sf::Color& borderColor)
{
	GuiImpl::Image(texture, size, uvRect, tintColor, borderColor);
}

void Gui::Image(const sf::Texture& texture, const sf::Vector2f& size, const sf::Vector2f& uv0, const sf::Vector2f& uv1,
                const sf::Color& tintColor, const sf::Color& borderColor)
{
	GuiImpl::Image(texture, size, uv0, uv1, tintColor, borderColor);
}

void Gui::Image(const sf::Sprite& sprite, const sf::Color& tintColor, const sf::Color& borderColor)
{
	GuiImpl::Image(sprite, tintColor, borderColor);
}

void Gui::Image(const sf::Sprite& sprite, const sf::Vector2f& size, const sf::Color& tintColor,
                const sf::Color& borderColor)
{
	GuiImpl::Image(sprite, size, tintColor, borderColor);
}

auto Gui::ImageButton(const sf::Texture& texture, int framePadding, const sf::Color& bgColor,
                      const sf::Color& tintColor) -> bool
{
	return GuiImpl::ImageButton(texture, framePadding, bgColor, tintColor);
}

auto Gui::ImageButton(const sf::Texture& texture, const sf::Vector2f& size, int framePadding, const sf::Color& bgColor,
                      const sf::Color& tintColor) -> bool
{
	return GuiImpl::ImageButton(texture, size, framePadding, bgColor, tintColor);
}

auto Gui::ImageButton(const sf::Sprite& sprite, int framePadding, const sf::Color& bgColor,
                      const sf::Color& tintColor) -> bool
{
	return GuiImpl::ImageButton(sprite, framePadding, bgColor, tintColor);
}

auto Gui::ImageButton(const sf::Sprite& sprite, const sf::Vector2f& size, int framePadding, const sf::Color& bgColor,
                      const sf::Color& tintColor) -> bool
{
	return GuiImpl::ImageButton(sprite, size, framePadding, bgColor, tintColor);
}

void Gui::HelpMarker(const String& desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc.c_str());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void Gui::InfoModal(const char* title, const char* text, bool& open)
{
	const auto prevFontSize = FontSize();
	SetFontSize(20);
	if (open && !ImGui::IsPopupOpen(title))
	{
		ImGui::OpenPopup(title);
	}

	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(title, nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(text);
		if (ImGui::Button("Dismiss"))
		{
			ImGui::CloseCurrentPopup();
		}
		open = false;
		ImGui::EndPopup();
	}
	SetFontSize(prevFontSize);
}

auto Gui::FontSize() -> int
{
	return Instance()._currentFont.first;
}

void Gui::SetStyle(GuiStyle guiStyle)
{
	ImGuiStyle& imguiStyle = ImGui::GetStyle();
	auto& currentStyle = Instance()._currentStyle;

	if (currentStyle != guiStyle)
	{
		switch (guiStyle)
		{
		case GuiStyle::Dark:
		{
			ImGui::GetStyle().FrameRounding = 4.0f;
			ImGui::GetStyle().GrabRounding = 4.0f;

			ImVec4* colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
			colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
			colors[ImGuiCol_Tab] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
			colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
			break;
		}
		case GuiStyle::Light:
		{
			ImGui::StyleColorsLight();

			auto& style = ImGui::GetStyle();
			auto* colors = style.Colors;
			colors[ImGuiCol_ChildBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);

			break;
		}
		case GuiStyle::VisualStudio:
		{
			constexpr auto ColorFromBytes = [](uint8_t r, uint8_t g, uint8_t b)
			{
				return ImVec4(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f,
				              static_cast<float>(b) / 255.0f, 1.0f);
			};

			auto& style = ImGui::GetStyle();
			ImVec4* colors = style.Colors;

			const ImVec4 bgColor = ColorFromBytes(37, 37, 38);
			const ImVec4 lightBgColor = ColorFromBytes(82, 82, 85);
			const ImVec4 veryLightBgColor = ColorFromBytes(90, 90, 95);

			const ImVec4 panelColor = ColorFromBytes(51, 51, 55);
			const ImVec4 panelHoverColor = ColorFromBytes(29, 151, 236);
			const ImVec4 panelActiveColor = ColorFromBytes(0, 119, 200);

			const ImVec4 textColor = ColorFromBytes(255, 255, 255);
			const ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
			const ImVec4 borderColor = ColorFromBytes(78, 78, 78);

			colors[ImGuiCol_Text] = textColor;
			colors[ImGuiCol_TextDisabled] = textDisabledColor;
			colors[ImGuiCol_TextSelectedBg] = panelActiveColor;
			colors[ImGuiCol_WindowBg] = bgColor;
			colors[ImGuiCol_ChildBg] = bgColor;
			colors[ImGuiCol_PopupBg] = bgColor;
			colors[ImGuiCol_Border] = borderColor;
			colors[ImGuiCol_BorderShadow] = borderColor;
			colors[ImGuiCol_FrameBg] = panelColor;
			colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
			colors[ImGuiCol_FrameBgActive] = panelActiveColor;
			colors[ImGuiCol_TitleBg] = bgColor;
			colors[ImGuiCol_TitleBgActive] = bgColor;
			colors[ImGuiCol_TitleBgCollapsed] = bgColor;
			colors[ImGuiCol_MenuBarBg] = panelColor;
			colors[ImGuiCol_ScrollbarBg] = panelColor;
			colors[ImGuiCol_ScrollbarGrab] = lightBgColor;
			colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
			colors[ImGuiCol_ScrollbarGrabActive] = veryLightBgColor;
			colors[ImGuiCol_CheckMark] = panelActiveColor;
			colors[ImGuiCol_SliderGrab] = ImVec4(0.10f, 0.42f, 0.64f, 1.0f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.32f, 0.54f, 1.0f);
			colors[ImGuiCol_Button] = panelColor;
			colors[ImGuiCol_ButtonHovered] = panelHoverColor;
			colors[ImGuiCol_ButtonActive] = panelHoverColor;
			colors[ImGuiCol_Header] = panelColor;
			colors[ImGuiCol_HeaderHovered] = panelHoverColor;
			colors[ImGuiCol_HeaderActive] = panelActiveColor;
			colors[ImGuiCol_Separator] = borderColor;
			colors[ImGuiCol_SeparatorHovered] = borderColor;
			colors[ImGuiCol_SeparatorActive] = borderColor;
			colors[ImGuiCol_ResizeGrip] = bgColor;
			colors[ImGuiCol_ResizeGripHovered] = panelColor;
			colors[ImGuiCol_ResizeGripActive] = lightBgColor;
			colors[ImGuiCol_PlotLines] = panelActiveColor;
			colors[ImGuiCol_PlotLinesHovered] = panelHoverColor;
			colors[ImGuiCol_PlotHistogram] = panelActiveColor;
			colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
			colors[ImGuiCol_DragDropTarget] = bgColor;
			colors[ImGuiCol_NavHighlight] = bgColor;
			colors[ImGuiCol_DockingPreview] = panelActiveColor;
			colors[ImGuiCol_Tab] = bgColor;
			colors[ImGuiCol_TabActive] = panelActiveColor;
			colors[ImGuiCol_TabUnfocused] = bgColor;
			colors[ImGuiCol_TabUnfocusedActive] = panelActiveColor;
			colors[ImGuiCol_TabHovered] = panelHoverColor;

			style.WindowRounding = 0.0f;
			style.ChildRounding = 0.0f;
			style.FrameRounding = 0.0f;
			style.GrabRounding = 0.0f;
			style.PopupRounding = 0.0f;
			style.ScrollbarRounding = 0.0f;
			style.TabRounding = 0.0f;
			break;
		}
		}
	}
	currentStyle = guiStyle;
}

void Gui::SetFontSize(int size)
{
	ImFont* candidate = AppropriateFont(size);
	Debug::Assert(candidate, "Failed to fetch appropriate font and could be caused by an empty font container");
	ImGui::SetCurrentFont(candidate);
}

auto Gui::AddFont(const Path& path, int size) -> Font*
{
	auto* newFont = ImGui::GetIO().Fonts->AddFontFromFileTTF(path.string().c_str(), static_cast<float>(size));
	Instance()._fonts.emplace(size, newFont);
	return newFont;
}

void Gui::ForceHideBarTab()
{
	if (ImGui::IsWindowDocked())
	{
		auto* node = ImGui::GetWindowDockNode();
		if (node)
		{
			if (node && !node->IsHiddenTabBar())
			{
				node->WantHiddenTabBarToggle = true;
			}
		}
	}
}

auto Gui::SaffronOrange(float opacity) -> sf::Vector4f
{
	return sf::Vector4f(0.89f, 0.46f, 0.16f, opacity);
}

auto Gui::SaffronPurple(float opacity) -> sf::Vector4f
{
	return sf::Vector4f(0.29f, 0.13f, 0.42f, opacity);
}

void Gui::PushID()
{
	ImGui::PushID(s_UIContextID++);
}

void Gui::PopID()
{
	ImGui::PopID();
	s_UIContextID--;
}

auto Gui::AppropriateFont(int size) -> Font*
{
	ImFont* candidate = nullptr;
	int bestDiff = std::numeric_limits<int>::max();
	for (auto& [fontSize, font] : Instance()._fonts)
	{
		if (std::abs(fontSize - size) > bestDiff)
		{
			break;
		}
		bestDiff = std::abs(fontSize - size);
		candidate = font;
	}
	return candidate;
}

auto Gui::ToImGuiSliderFlags(GuiPropertyFlag flags) -> ImGuiSliderFlags
{
	ImGuiSliderFlags imGuiSliderFlags = 0;
	if (flags & GuiPropertyFlag_Logarithmic)
	{
		imGuiSliderFlags |= ImGuiSliderFlags_Logarithmic;
	}
	return imGuiSliderFlags;
}
}
