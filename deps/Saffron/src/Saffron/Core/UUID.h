#pragma once

#undef UUID_DEFINED

#include "Saffron/Core/TypeDefs.h"

namespace Se
{
class UUID
{
public:
	UUID();
	UUID(Uint64 uuid);
	UUID(const UUID& other);

	operator Uint64() { return _uuid; }

	operator const Uint64() const { return _uuid; }

	auto operator==(const UUID& other) const -> bool
	{
		return _uuid == other._uuid;
	}

	static auto Null() -> UUID { return UUID(0ull); }

private:
	Uint64 _uuid;
};
}

namespace std
{
template <>
struct hash<Se::UUID>
{
	auto operator()(const Se::UUID& uuid) const -> std::size_t
	{
		return hash<Se::Uint64>()(static_cast<Se::Uint64>(uuid));
	}
};
}
