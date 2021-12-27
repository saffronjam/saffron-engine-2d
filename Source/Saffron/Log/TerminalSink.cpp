#include "SaffronPCH.h"

#include "Saffron/Log/TerminalSink.h"

namespace Se
{
TerminalSink::TerminalSink()
{
	Log::ClientLogger()->OnAll += SE_EV_FUNCTION(TerminalSink::Post);
}

void TerminalSink::Clear()
{
	_textBuffer.clear();
	_lineOffsets.clear();
	_lineOffsets.push_back(0);
}

auto TerminalSink::TextBuffer() const -> const ImGuiTextBuffer&
{
	return _textBuffer;
}

auto TerminalSink::LineOffsets() const -> const std::vector<int>&
{
	return _lineOffsets;
}

bool TerminalSink::Post(const LogMessage& message)
{
	int old_size = _textBuffer.size();
	_textBuffer.append(message.Formatted.c_str());
	for (const int new_size = _textBuffer.size(); old_size < new_size; old_size++)
	{
		if (_textBuffer[old_size] == '\n')
		{
			_lineOffsets.push_back(old_size + 1);
		}
	}
	return false;
}
}
