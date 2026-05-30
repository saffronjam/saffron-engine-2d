#include "saffron_pch.h"

#include "saffron/core/uuid.h"
#include "saffron/math/random.h"

namespace saffron
{
static RandomGenerator<ulong> sRandom(0, std::numeric_limits<ulong>::max());


UUID::UUID() :
	_uuid(sRandom.Generate())
{
}

UUID::UUID(ulong uuid) :
	_uuid(uuid)
{
}

UUID::UUID(const UUID& other) :
	UUID(other._uuid)
{
}

auto UUID::Value() const -> ulong
{
	return _uuid;
}

auto UUID::IsEqualTo(const UUID& other) const -> bool
{
	return _uuid == other._uuid;
}
}
