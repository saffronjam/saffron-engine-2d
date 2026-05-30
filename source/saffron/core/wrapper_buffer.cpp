#include "saffron_pch.h"

#include "saffron/core/wrapper_buffer.h"
#include "saffron/log/log.h"

namespace saffron
{
WrapperBuffer::WrapperBuffer() :
	WrapperBuffer(nullptr, 0)
{
}

WrapperBuffer::WrapperBuffer(uchar* data, uint size) :
	_data(data),
	_size(size)
{
}

auto WrapperBuffer::Copy(const void* data, uint size) -> WrapperBuffer
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

auto WrapperBuffer::Encapsulate(uchar* data) -> WrapperBuffer
{
	WrapperBuffer buffer;
	buffer._data = data;
	buffer._size = 0;
	return buffer;
}

void WrapperBuffer::Allocate(uint size)
{
	if (_data)
	{
		delete[] _data;
		_data = nullptr;
	}

	if (size == 0) return;

	_data = new uchar[size];
	_size = size;
}

void WrapperBuffer::ZeroInitialize() const
{
	if (_data) memset(_data, 0, _size);
}

void WrapperBuffer::Write(void* data, uint size, uint offset) const
{
	if (offset + size > _size)
	{
		Log::CoreError("WrapperBuffer overflow");
		return;
	}
	memcpy(_data + offset, data, size);
}

void WrapperBuffer::Write(const void* data, uint size, uint offset) const
{
	if (offset + size > _size)
	{
		Log::CoreError("WrapperBuffer overflow");
		return;
	}
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

auto WrapperBuffer::IsValid() const -> bool
{
	return _data != nullptr;
}

auto WrapperBuffer::At(int index) -> uchar&
{
	return _data[index];
}

auto WrapperBuffer::At(int index) const -> uchar
{
	return _data[index];
}

auto WrapperBuffer::Data() -> uchar*
{
	return _data;
}

auto WrapperBuffer::Data() const -> const uchar*
{
	return _data;
}

auto WrapperBuffer::Size() const -> uint
{
	return _size;
}
}
