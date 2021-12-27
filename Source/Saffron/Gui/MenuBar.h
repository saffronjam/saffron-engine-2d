#pragma once

#include "Saffron/Base.h"

#undef AppendMenu

namespace Se
{
struct MenuBarMenu
{
	std::string Title;
	std::function<void()> Generator;
};

class MenuBar : public Singleton<MenuBar>
{
public:
	MenuBar();

	void Begin() const;
	void End();

	static void AddMenu(MenuBarMenu menu, int order = 0);

private:
	std::multimap<int, MenuBarMenu> _menus;
};
}
