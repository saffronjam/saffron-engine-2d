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
	std::unique_ptr<Log> _log;
};
}
