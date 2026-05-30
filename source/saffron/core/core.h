#pragma once

#include "saffron/core/type_defs.h"
#include "saffron/log/log.h"

namespace saffron
{
class Core
{
public:
	Core();

private:
	std::unique_ptr<Log> _log;
};
}
