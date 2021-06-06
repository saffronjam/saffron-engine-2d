#pragma once

#include "Saffron/Gui/Gui.h"
#include "Saffron/Log/LogSink.h"

namespace Se
{
class TerminalSink : public LogSink
{
public:
	void Clear();

	auto GetTextBuffer() const -> const ImGuiTextBuffer& { return _textBuffer; }

	auto GetLineOffsets() const -> const ArrayList<int>& { return _lineOffsets; }

protected:
	void Sink(const LogMessage& message) override;
	void Flush() override;

private:
	ImGuiTextBuffer _textBuffer;
	ArrayList<int> _lineOffsets;
};
}
