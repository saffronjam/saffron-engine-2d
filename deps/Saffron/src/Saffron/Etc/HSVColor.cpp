#include "SaffronPCH.h"

#include "Saffron/Etc/HSVColor.h"
#include "Saffron/Libraries/GenUtils.h"

namespace Se
{
HSVColor::HSVColor() :
	HSVColor(0, 0.0f, 0.0f)
{
}

HSVColor::HSVColor(int hue, float saturation, float value) :
	Color(GenUtils::HSVtoRGB(hue, saturation, value)),
	_hue(hue),
	_saturation(saturation),
	_value(value)
{
}

HSVColor::HSVColor(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 a) :
	HSVColor(Color(r, g, b, a))
{
}

HSVColor::HSVColor(const Color& color) :
	HSVColor(GenUtils::RGBtoHSV(color))
{
}

HSVColor::HSVColor(const HSVColor& hsvColor) :
	HSVColor(hsvColor._hue, hsvColor._saturation, hsvColor._value)
{
}

auto HSVColor::RGBA() const -> const sf::Color&
{
	return *this;
}

auto HSVColor::Hue() const -> int
{
	return _hue;
}

auto HSVColor::Saturation() const -> float
{
	return _saturation;
}

auto HSVColor::Value() const -> float
{
	return _value;
}

void HSVColor::SetHue(int hue)
{
	_hue = hue;
	GenerateRGB();
}

void HSVColor::SetSaturation(float saturation)
{
	_saturation = saturation;
	GenerateRGB();
}

void HSVColor::SetValue(float value)
{
	_value = value;
	GenerateRGB();
}

void HSVColor::GenerateRGB()
{
	const auto color = GenUtils::HSVtoRGB(_hue, _saturation, _value);
	r = color.r;
	g = color.g;
	b = color.b;
}
}
