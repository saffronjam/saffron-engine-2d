#ifndef IMGUI_SFML_H
#define IMGUI_SFML_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Joystick.hpp>

namespace sf
{
class Event;
class RenderTarget;
class RenderWindow;
class Sprite;
class Texture;
class Window;
}

namespace GuiImpl
{
namespace SFML
{
void Init(sf::RenderWindow& window, bool loadDefaultFont);
void Init(sf::Window& window, sf::RenderTarget& target, bool loadDefaultFont);
void Init(sf::Window& window, const sf::Vector2f& displaySize, bool loadDefaultFont);

void ProcessEvent(const sf::Event& event);

void Update(sf::RenderWindow& window, sf::Time dt);
void Update(sf::Window& window, sf::RenderTarget& target, sf::Time dt);
void Update(const sf::Vector2i& mousePos, const sf::Vector2f& displaySize, sf::Time dt);

void Render(sf::RenderTarget& target);
void Render();

void Shutdown();

void UpdateFontTexture();
auto GetFontTexture() -> sf::Texture&;

// joystick functions
void SetActiveJoystickId(unsigned int joystickId);
void SetJoytickDPadThreshold(float threshold);
void SetJoytickLStickThreshold(float threshold);

void SetJoystickMapping(int action, unsigned int joystickButton);
void SetDPadXAxis(sf::Joystick::Axis dPadXAxis, bool inverted);
void SetDPadYAxis(sf::Joystick::Axis dPadYAxis, bool inverted);
void SetLStickXAxis(sf::Joystick::Axis lStickXAxis, bool inverted);
void SetLStickYAxis(sf::Joystick::Axis lStickYAxis, bool inverted);
}

// custom ImGui widgets for SFML stuff

// Image overloads
void Image(const sf::Texture& texture, const sf::Color& tintColor,
           const sf::Color& borderColor);
void Image(const sf::Texture& texture, const sf::Vector2f& size, const sf::Color& tintColor,
           const sf::Color& borderColor);
void Image(const sf::Texture& texture, const sf::FloatRect& uvRect, const sf::Color& tintColor,
           const sf::Color& borderColor);
void Image(const sf::Texture& texture, const sf::Vector2f& size, const sf::FloatRect& uvRect,
           const sf::Color& tintColor, const sf::Color& borderColor);
void Image(const sf::Texture& texture, const sf::Vector2f& size, const sf::Vector2f& uv0, const sf::Vector2f& uv1,
           const sf::Color& tintColor, const sf::Color& borderColor);

void Image(const sf::Sprite& sprite, const sf::Color& tintColor,
           const sf::Color& borderColor);
void Image(const sf::Sprite& sprite, const sf::Vector2f& size, const sf::Color& tintColor,
           const sf::Color& borderColor);

// ImageButton overloads
auto ImageButton(const sf::Texture& texture, int framePadding, const sf::Color& bgColor,
                 const sf::Color& tintColor) -> bool;
auto ImageButton(const sf::Texture& texture, const sf::Vector2f& size, int framePadding,
                 const sf::Color& bgColor,
                 const sf::Color& tintColor) -> bool;

auto ImageButton(const sf::Sprite& sprite, int framePadding, const sf::Color& bgColor,
                 const sf::Color& tintColor) -> bool;
auto ImageButton(const sf::Sprite& sprite, const sf::Vector2f& size, int framePadding,
                 const sf::Color& bgColor,
                 const sf::Color& tintColor) -> bool;


// Draw_list overloads. All positions are in relative coordinates (relative to top-left of the current window)
void DrawLine(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Color& col, float thickness);
void DrawRect(const sf::FloatRect& rect, const sf::Color& color, float rounding, int rounding_corners,
              float thickness);
void DrawRectFilled(const sf::FloatRect& rect, const sf::Color& color, float rounding,
                    int rounding_corners);
}

#endif //# IMGUI_SFML_H
