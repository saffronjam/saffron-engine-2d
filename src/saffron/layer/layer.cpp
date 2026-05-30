#include "saffron_pch.h"

#include "saffron/layer/layer.h"

namespace saffron
{
LayerImpl::LayerImpl(const std::string& name) :
	_debugName(name)
{
}

auto LayerImpl::Name() const -> const std::string&
{
	return _debugName;
}
}
