#pragma once

#include "Saffron/Base.h"

#undef AppendMenu

namespace Se
{

struct MenuBarMenu
{
	String Title;
	Function<void()> Generator;
};

class MenuBar : public SingleTon<MenuBar>
{
public:
	MenuBar();

	void Begin() const;
	void End();

	static void AddMenu(MenuBarMenu menu, int order = 0);
	
private:
	MultiTreeMap<int, MenuBarMenu> _menus;
};
}
