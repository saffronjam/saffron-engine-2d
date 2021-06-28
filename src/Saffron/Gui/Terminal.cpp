#include "SaffronPCH.h"

#include "Saffron/Gui/Terminal.h"
#include "Saffron/Gui/Gui.h"

namespace Se
{
// TODO: Implement as a GUI interface
static ulong s_GuiID = 0;

Terminal::Terminal()
{
	_sink = std::make_shared<TerminalSink>();
	Clear();
	//Log::AddClientSink(_sink);
}

void Terminal::Clear() const
{
	_sink->Clear();
}

void Terminal::OnGuiRender()
{
	OStringStream oss;
	oss << "Terminal##" << s_GuiID++;

	if (!ImGui::Begin("Terminal"))
	{
		ImGui::End();
		return;
	}

	// Options menu
	if (ImGui::BeginPopup("Options"))
	{
		ImGui::Checkbox("Auto-scroll", &_autoScroll);
		ImGui::EndPopup();
	}

	// Main window
	if (ImGui::Button("Options")) ImGui::OpenPopup("Options");
	ImGui::SameLine();
	const bool clear = ImGui::Button("Clear");
	ImGui::SameLine();
	const bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	_filter.Draw("Filter", -100.0f);

	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (clear) Clear();
	if (copy) ImGui::LogToClipboard();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	const char* buf = _sink->TextBuffer().begin();
	const char* buf_end = _sink->TextBuffer().end();
	if (_filter.IsActive())
	{
		for (int line_no = 0; line_no < _sink->LineOffsets().size(); line_no++)
		{
			const char* line_start = buf + _sink->LineOffsets()[line_no];
			const char* line_end = line_no + 1 < _sink->LineOffsets().size()
				                       ? buf + _sink->LineOffsets()[line_no + 1] - 1
				                       : buf_end;
			if (_filter.PassFilter(line_start, line_end)) ImGui::TextUnformatted(line_start, line_end);
		}
	}
	else
	{
		ImGuiListClipper clipper;
		clipper.Begin(_sink->LineOffsets().size());
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				const char* line_start = buf + _sink->LineOffsets()[line_no];
				const char* line_end = line_no + 1 < _sink->LineOffsets().size()
					                       ? buf + _sink->LineOffsets()[line_no + 1] - 1
					                       : buf_end;
				ImGui::TextUnformatted(line_start, line_end);
			}
		}
		clipper.End();
	}
	ImGui::PopStyleVar();

	if (_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();
	ImGui::End();

	s_GuiID--;
}

}
