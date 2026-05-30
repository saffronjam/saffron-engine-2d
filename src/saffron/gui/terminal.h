#pragma once

#include "saffron/log/terminal_sink.h"

namespace saffron
{
class Terminal
{
public:
	Terminal();

	void Clear() const;
	void OnGuiRender();

private:
	std::shared_ptr<TerminalSink> _sink;
	ImGuiTextFilter _filter;
	bool _autoScroll = true;
};
}
