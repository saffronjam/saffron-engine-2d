#pragma once

#undef UUID_DEFINED

#include "saffron/core/type_defs.h"

namespace saffron
{
class UUID
{
public:
	UUID();
	UUID(ulong uuid);
	UUID(const UUID& other);

	auto Value() const -> ulong;
	auto IsEqualTo(const UUID& other) const -> bool;

	static auto Null() -> UUID { return UUID(0ull); }

private:
	ulong _uuid;
};
}
