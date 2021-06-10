#include "Layers/ProjectLayer.h"

#include <SFML/Graphics.hpp>

namespace Se
{
void ProjectLayer::OnAttach(std::shared_ptr<BatchLoader>& loader)
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


	sf::RectangleShape shape(sf::Vector2f{10.0f, 10.0f});

	static int count = 0;

	if (Keyboard::IsPressed(sf::Keyboard::G))
	{
		count -= 50;
		count = std::max(0, count);
	}
	if (Keyboard::IsPressed(sf::Keyboard::H))
	{
		count += 1000;
		count = std::max(0, count);
	}

	for (int i = 0; i < count; i++)
	{
		_scene.Submit(shape);
		shape.setPosition(10.0f * std::cos(2 * 3.1415f / 45.0f * float(i)), 10.0f * std::sin(2 * 3.1415f / 45.0f * float(i)));
	}
}

void ProjectLayer::OnGuiRender()
{
	BaseLayer::OnGuiRender();

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Gui"))
		{
			ImGui::MenuItem("View System", nullptr, &_viewSystem);
			ImGui::MenuItem("View Demo", nullptr, &_viewDemo);
			if (ImGui::BeginMenu("Theme"))
			{
#define MENU_ITEM(guiStyle) if(ImGui::MenuItem(#guiStyle)) { Gui::SetStyle(GuiStyle:: ## guiStyle); }
				MENU_ITEM(Dark)
				MENU_ITEM(Light)
				MENU_ITEM(VisualStudio)
#undef MENU_ITEM
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::Begin("Project"))
	{
	}
	ImGui::End();

	if (_viewDemo)
	{
		ImGui::ShowDemoWindow();
	}
}

void ProjectLayer::OnRenderTargetResize(const sf::Vector2f& newSize)
{
	BaseLayer::OnRenderTargetResize(newSize);
}
}
