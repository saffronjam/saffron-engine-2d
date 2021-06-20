#pragma once

#ifdef _MSC_VER
#pragma warning(disable: 4244)
#pragma warning(disable: 4715)
#endif

#include "Saffron/Core/Core.h"

extern Se::Unique<Se::App> Se::CreateApplication();

int main(int argc, char **argv)
{
	Se::Core _core;
	auto app = Se::CreateApplication();
	Se::Debug::Assert(app.get(), "Client Application is null!");
	app->Run();
}
