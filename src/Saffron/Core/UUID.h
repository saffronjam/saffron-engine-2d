#pragma once

#undef UUID_DEFINED

#include "Saffron/Core/TypeDefs.h"

namespace Se
{
class UUID
{
public:
	UUID();
	UUID(ulong uuid);
	UUID(const UUID& other);

	operator ulong() { return _uuid; }

	operator const ulong() const { return _uuid; }

	auto operator==(const UUID& other) const -> bool
	{
		return _uuid == other._uuid;
	}

	static auto Null() -> UUID { return UUID(0ull); }

private:
	ulong _uuid;
};
}

namespace std
{
template <>
struct hash<Se::UUID>
{
	auto operator()(const Se::UUID& uuid) const -> std::size_t
	{
		return hash<Se::ulong>()(static_cast<Se::ulong>(uuid));
	}
};
}
