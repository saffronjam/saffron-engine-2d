#pragma once

#include "Saffron/Log/TerminalSink.h"

namespace Se
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
