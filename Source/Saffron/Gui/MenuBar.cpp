#include "SaffronPCH.h"

#include "Saffron/Gui/MenuBar.h"
#include "Saffron/Gui/Gui.h"

namespace Se
{
MenuBar::MenuBar():
	Singleton<MenuBar>(this)
{
}

void MenuBar::Begin() const
{
	if (ImGui::BeginMenuBar())
	{
		for (const auto& menu : _menus | std::views::values)
		{			
			if(ImGui::BeginMenu(menu.Title.c_str()))
			{
				menu.Generator();
				ImGui::EndMenu();
			}
		}
		ImGui::EndMenuBar();
	}
}

void MenuBar::End()
{
}

void MenuBar::AddMenu(MenuBarMenu menu, int order)
{
	Instance()._menus.emplace(order, std::move(menu));
}
}
