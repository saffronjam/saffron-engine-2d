#include "saffron_pch.h"

#include "saffron/core/core.h"
#include "saffron/log/log.h"

namespace saffron
{
Core::Core() :
	_log(std::make_unique<Log>())
{
}
}
