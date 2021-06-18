#pragma once

#include "Saffron/Core/TypeDefs.h"

namespace Se
{
class WrapperBuffer
{
public:
	WrapperBuffer();
	WrapperBuffer(Uint8* data, Uint32 size);

	static auto Copy(const void* data, Uint32 size) -> WrapperBuffer;
	static auto Copy(const WrapperBuffer& buffer) -> WrapperBuffer;
	static auto Encapsulate(Uint8* data) -> WrapperBuffer;

	void Allocate(Uint32 size);
	void ZeroInitialize() const;

	template <typename T>
	auto Read(Uint32 offset = 0) -> T&;

	void Write(void* data, Uint32 size, Uint32 offset = 0) const;
	void Write(const void* data, Uint32 size, Uint32 offset) const;

	void Destroy();
	void Reset();

	operator bool() const;
	auto operator[](int index) -> Uint8&;
	auto operator[](int index) const -> Uint8;

	template <typename T>
	auto As() -> T*;

	auto Data() -> Uint8*;
	auto Data() const -> const Uint8*;
	auto Size() const -> Uint32;

private:
	Uint8* _data;
	Uint32 _size;
};


template <typename T>
auto WrapperBuffer::Read(Uint32 offset) -> T&
{
	return *reinterpret_cast<T*>(_data + offset);
}

template <typename T>
auto WrapperBuffer::As() -> T*
{
	return reinterpret_cast<T*>(_data);
}
}
