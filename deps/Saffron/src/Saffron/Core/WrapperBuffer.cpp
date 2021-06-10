#include "SaffronPCH.h"

#include "Saffron/Debug/Debug.h"
#include "Saffron/Core/WrapperBuffer.h"

namespace Se
{
WrapperBuffer::WrapperBuffer() :
	WrapperBuffer(nullptr, 0)
{
}

WrapperBuffer::WrapperBuffer(Uint8* data, Uint32 size) :
	_data(data),
	_size(size)
{
}

auto WrapperBuffer::Copy(const void* data, Uint32 size) -> WrapperBuffer
{
	WrapperBuffer buffer;
	buffer.Allocate(size);
	memcpy(buffer._data, data, size);
	return buffer;
}

auto WrapperBuffer::Copy(const WrapperBuffer& buffer) -> WrapperBuffer
{
	return Copy(buffer.Data(), buffer.Size());
}

auto WrapperBuffer::Encapsulate(Uint8* data) -> WrapperBuffer
{
	WrapperBuffer buffer;
	buffer._data = data;
	buffer._size = 0;
	return buffer;
}

void WrapperBuffer::Allocate(Uint32 size)
{
	if (_data)
	{
		delete[] _data;
		_data = nullptr;
	}

	if (size == 0) return;

	_data = new Uint8[size];
	_size = size;
}

void WrapperBuffer::ZeroInitialize() const
{
	if (_data) memset(_data, 0, _size);
}

void WrapperBuffer::Write(void* data, Uint32 size, Uint32 offset) const
{
	Debug::Assert(offset + size <= _size, "WrapperBuffer overflow");
	memcpy(_data + offset, data, size);
}

void WrapperBuffer::Write(const void* data, Uint32 size, Uint32 offset) const
{
	Debug::Assert(offset + size <= _size, "WrapperBuffer overflow");
	memcpy(_data + offset, data, size);
}

void WrapperBuffer::Destroy()
{
	if (_data)
	{
		delete[] _data;
	}
	Reset();
}

void WrapperBuffer::Reset()
{
	_data = nullptr;
	_size = 0;
}

WrapperBuffer::operator bool() const
{
	return _data;
}

auto WrapperBuffer::operator[](int index) -> Uint8&
{
	return _data[index];
}

auto WrapperBuffer::operator[](int index) const -> Uint8
{
	return _data[index];
}

auto WrapperBuffer::Data() -> Uint8*
{
	return _data;
}

auto WrapperBuffer::Data() const -> const Uint8*
{
	return _data;
}

auto WrapperBuffer::Size() const -> Uint32
{
	return _size;
}
}
