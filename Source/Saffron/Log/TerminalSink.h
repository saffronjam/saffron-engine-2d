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
	auto LineOffsets() const -> const List<int>&;

protected:
	bool Post(const LogMessage &message);

private:
	ImGuiTextBuffer _textBuffer;
	List<int> _lineOffsets;
};
}
