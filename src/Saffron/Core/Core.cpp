#include "SaffronPCH.h"

#include "Saffron/Core/Core.h"
#include "Saffron/Log/Log.h"

namespace Se
{
Core::Core() :
	_log(std::make_unique<Log>())
{
}
}
