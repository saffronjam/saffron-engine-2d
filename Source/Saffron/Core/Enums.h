#pragma once

namespace Se
{
enum class TextAlign
{
	Left,
	Middle,
	Right
};

enum class GuiStyle : int
{
	Dark = 0,
	Light = 1,
	VisualStudio = 2
};

typedef uint GuiPropertyFlag;

enum GuiPropertyFlag_ : uint
{
	GuiPropertyFlag_None = 0u,
	GuiPropertyFlag_Color = 1u << 4u,
	GuiPropertyFlag_Drag = 1u << 5u,
	GuiPropertyFlag_Slider = 1u << 6u,
	GuiPropertyFlag_Logarithmic = 1u << 7u
};

enum class FadeType
{
	Out = 0,
	In = 1
};
}
