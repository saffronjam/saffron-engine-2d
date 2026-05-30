#pragma once

#ifdef _MSC_VER
#pragma warning(disable: 4244)
#pragma warning(disable: 4715)
#endif

#include "saffron/core/core.h"

extern std::unique_ptr<saffron::App> saffron::CreateApplication();

int main(int argc, char **argv)
{
	saffron::Core _core;
	auto app = saffron::CreateApplication();
	if (!app)
	{
		return 1;
	}
	app->Run();
	return 0;
}
