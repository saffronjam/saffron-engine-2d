#include "ProjectLayer.h"

namespace Se
{
void ProjectLayer::OnAttach(std::shared_ptr<BatchLoader> &loader)
{
	BaseLayer::OnAttach(loader);
}

void ProjectLayer::OnDetach()
{
	BaseLayer::OnDetach();
}

void ProjectLayer::OnUpdate()
{
	BaseLayer::OnUpdate();
}

void ProjectLayer::OnGuiRender()
{
	BaseLayer::OnGuiRender();

	if ( ImGui::Begin("Project") )
	{
		Gui::BeginPropertyGrid();
		Gui::EndPropertyGrid();
	}
	ImGui::End();

	ImGui::ShowDemoWindow();
}
}
