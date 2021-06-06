#pragma once

#include "Saffron/Core/TypeDefs.h"
#include "Saffron/Log/Log.h"

namespace Se
{
class Core
{
public:
	Core();

private:
	Unique<Log> _log;
};
}
