#pragma once

#include <SFML/Graphics/Color.hpp>


namespace Se
{
class HSVColor : public sf::Color
{
public:
	HSVColor();
	HSVColor(int hue, float saturation, float value);
	HSVColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a = 255);
	HSVColor(const Color& color);
	HSVColor(const HSVColor& hsvColor);

	auto GetRGBA() const -> const sf::Color&;

	auto GetHue() const -> int;
	auto GetSaturation() const -> float;
	auto GetValue() const -> float;

	void SetHue(int hue);
	void SetSaturation(float saturation);
	void SetValue(float value);

private:
	void GenerateRGB();

private:
	int _hue;
	float _saturation, _value;
};
}
