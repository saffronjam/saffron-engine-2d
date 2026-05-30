#pragma once

#include <imgui.h>
#include <imgui_internal.h>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>

#include "saffron/base.h"
#include "saffron/core/enums.h"
#include "saffron/math/saffron_math.h"

namespace saffron
{
using Font = ImFont;

class Gui : public Singleton<Gui>
{
public:
	explicit Gui();
	virtual ~Gui();

	void OnGuiRender();

	void Begin();
	void End();

	static void BeginPropertyGrid(std::string id, float width);
	static void EndPropertyGrid();

	static auto BeginTreeNode(const std::string& name, bool defaultOpen) -> bool;
	static void EndTreeNode();

	////////////////////////////////
	/// Bool and string
	////////////////////////////////

	static void Property(const std::string& name, const std::function<void()>& onClick, bool secondColumn);

	static void Property(const std::string& name, const std::string& value);

	static auto Property(const std::string& name, std::string& value) -> bool;

	static auto Property(const std::string& name, bool& value) -> bool;

	static auto Property(const std::string& name, const std::string& text, const std::string& buttonName,
	                     const std::function<void()>& onButtonPress) -> bool;

	////////////////////////////////
	/// Single value
	////////////////////////////////

	static auto Property(const std::string& name, int& value, int min, int max, float step,
	                     GuiPropertyFlag flags) -> bool;

	static auto Property(const std::string& name, int& value, const char* format, int min, int max, float step,
	                     GuiPropertyFlag flags) -> bool;

	static auto Property(const std::string& name, float& value, float min, float max, float step,
	                     GuiPropertyFlag flags) -> bool;

	static auto Property(const std::string& name, float& value, const char* format, float min, float max,
	                     float step, GuiPropertyFlag flags) -> bool;

	////////////////////////////////
	/// Vector2
	////////////////////////////////

	static auto Property(const std::string& name, sf::Vector2f& value, GuiPropertyFlag flags) -> bool;

	static auto Property(const std::string& name, sf::Vector2f& value, float min, float max,
	                     float step, GuiPropertyFlag flags) -> bool;

	static auto Property(const std::string& name, sf::Vector2f& value, const char* format, float min,
	                     float max, float step, GuiPropertyFlag flags) -> bool;

	////////////////////////////////
	/// Vector3
	////////////////////////////////

	static auto Property(const std::string& name, sf::Vector3f& value, GuiPropertyFlag flags) -> bool;

	static auto Property(const std::string& name, sf::Vector3f& value, float min, float max,
	                     float step, GuiPropertyFlag flags,
	                     std::optional<std::function<void()>> fn) -> bool;

	static auto Property(const std::string& name, sf::Vector3f& value, const char* format, float min,
	                     float max, float step, GuiPropertyFlag flags,
	                     std::optional<std::function<void()>> fn) -> bool;

	////////////////////////////////
	/// Vector4
	////////////////////////////////

	static auto Property(const std::string& name, sf::Vector4f& value, GuiPropertyFlag flags) -> bool;

	static auto Property(const std::string& name, sf::Vector4f& value, float min, float max,
	                     float step, GuiPropertyFlag flags) -> bool;

	static auto Property(const std::string& name, sf::Vector4f& value, const char* format, float min,
	                     float max, float step, GuiPropertyFlag flags) -> bool;


	static void Image(const sf::Texture& texture, const sf::Color& tintColor,
	                  const sf::Color& borderColor);

	static void Image(const sf::Texture& texture, const sf::Vector2f& size,
	                  const sf::Color& tintColor,
	                  const sf::Color& borderColor);

	static void Image(const sf::Texture& texture, const sf::FloatRect& uvRect,
	                  const sf::Color& tintColor,
	                  const sf::Color& borderColor);

	static void Image(const sf::Texture& texture, const sf::Vector2f& size, const sf::FloatRect& uvRect,
	                  const sf::Color& tintColor,
	                  const sf::Color& borderColor);

	static void Image(const sf::Texture& texture, const sf::Vector2f& size, const sf::Vector2f& uv0,
	                  const sf::Vector2f& uv1, const sf::Color& tintColor,
	                  const sf::Color& borderColor);

	static void Image(const sf::Sprite& sprite, const sf::Color& tintColor,
	                  const sf::Color& borderColor);

	static void Image(const sf::Sprite& sprite, const sf::Vector2f& size, const sf::Color& tintColor,
	                  const sf::Color& borderColor);

	// ImageButton overloads
	static auto ImageButton(const sf::Texture& texture, int framePadding,
	                        const sf::Color& bgColor,
	                        const sf::Color& tintColor) -> bool;

	static auto ImageButton(const sf::Texture& texture, const sf::Vector2f& size, int framePadding,
	                        const sf::Color& bgColor,
	                        const sf::Color& tintColor) -> bool;

	static auto ImageButton(const sf::Sprite& sprite, int framePadding,
	                        const sf::Color& bgColor,
	                        const sf::Color& tintColor) -> bool;

	static auto ImageButton(const sf::Sprite& sprite, const sf::Vector2f& size, int framePadding,
	                        const sf::Color& bgColor,
	                        const sf::Color& tintColor) -> bool;

	static void HelpMarker(const std::string& desc);

	static void InfoModal(const char* title, const char* text, bool& open);

	static auto FontSize() -> int;
	static auto Style() -> GuiStyle;
	static void SetStyle(GuiStyle guiStyle);
	static void SetFontSize(int size);
	static auto AddFont(const std::filesystem::path& path, int size) -> Font*;

	static void ForceHideBarTab();

	static auto SaffronOrange(float opacity) -> sf::Vector4f;
	static auto SaffronPurple(float opacity) -> sf::Vector4f;

private:
	void OnRenderMenuBar();

private:
	static void PushID();
	static void PopID();

	static auto AppropriateFont(int size) -> Font*;

	static auto ToImGuiSliderFlags(GuiPropertyFlag flags) -> ImGuiSliderFlags;

private:
	GuiStyle _currentStyle = GuiStyle::Light;
	std::map<int, ImFont*> _fonts;
	std::pair<int, ImFont*> _currentFont;

	bool _viewDemo = true;
};
}
