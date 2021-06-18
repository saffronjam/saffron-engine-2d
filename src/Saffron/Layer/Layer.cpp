#include "SaffronPCH.h"

#include "Saffron/Layer/Layer.h"

namespace Se
{
Layer::Layer(const String& name) :
	_debugName(name)
{
}

auto Layer::Name() const -> const String&
{
	return _debugName;
}
}
