#pragma once

#include "Saffron/Gui/Gui.h"

namespace Se
{
class TerminalSink
{
public:
	TerminalSink();
	
	void Clear();

	auto TextBuffer() const -> const ImGuiTextBuffer&;
	auto LineOffsets() const -> const std::vector<int>&;

protected:
	bool Post(const LogMessage &message);

private:
	ImGuiTextBuffer _textBuffer;
	std::vector<int> _lineOffsets;
};
}
