#pragma once

#include "saffron/core/type_defs.h"

namespace saffron
{
class WrapperBuffer
{
public:
	WrapperBuffer();
	WrapperBuffer(uchar* data, uint size);

	static auto Copy(const void* data, uint size) -> WrapperBuffer;
	static auto Copy(const WrapperBuffer& buffer) -> WrapperBuffer;
	static auto Encapsulate(uchar* data) -> WrapperBuffer;

	void Allocate(uint size);
	void ZeroInitialize() const;

	template <typename T>
	auto Read(uint offset) -> T&;

	void Write(void* data, uint size, uint offset) const;
	void Write(const void* data, uint size, uint offset) const;

	void Destroy();
	void Reset();

	auto IsValid() const -> bool;
	auto At(int index) -> uchar&;
	auto At(int index) const -> uchar;

	template <typename T>
	auto As() -> T*;

	auto Data() -> uchar*;
	auto Data() const -> const uchar*;
	auto Size() const -> uint;

private:
	uchar* _data;
	uint _size;
};


template <typename T>
auto WrapperBuffer::Read(uint offset) -> T&
{
	return *reinterpret_cast<T*>(_data + offset);
}

template <typename T>
auto WrapperBuffer::As() -> T*
{
	return reinterpret_cast<T*>(_data);
}
}
