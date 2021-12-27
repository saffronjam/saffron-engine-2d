#include "SaffronPCH.h"

#include "Saffron/Layer/Layer.h"

namespace Se
{
Layer::Layer(const std::string& name) :
	_debugName(name)
{
}

auto Layer::Name() const -> const std::string&
{
	return _debugName;
}
}
