#include "ProjectLayer.h"

#include <SFML/Graphics.hpp>

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
		OutputStringStream oss;
		oss << Mouse::GetPosition().x << ", " << Mouse::GetPosition().y;
		Gui::BeginPropertyGrid();
		Gui::Property("Mouse pos", oss.str());
		Gui::EndPropertyGrid();
	}
	ImGui::End();



	ImGui::ShowDemoWindow();
}
}
