#pragma once

#include "saffron/base.h"

#undef AppendMenu

namespace saffron
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

	static void AddMenu(MenuBarMenu menu, int order);

private:
	std::multimap<int, MenuBarMenu> _menus;
};
}
