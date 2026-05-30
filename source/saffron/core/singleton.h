#pragma once

#include "saffron/core/non_copyable.h"

namespace saffron
{
template <class Class>
class Singleton : public NonCopyable
{
public:
	explicit Singleton(Class* instance);
	virtual ~Singleton();

	static auto Instance() -> Class&;

protected:
	static Class* _instance;
};

template <class Class>
Singleton<Class>::Singleton(Class* instance)
{
	if (_instance != nullptr)
	{
		std::cerr << typeid(Class).name() << " was already instantiated\n";
		std::abort();
	}
	_instance = instance;
}

template <class Class>
Singleton<Class>::~Singleton()
{
	_instance = nullptr;
}

template <class Class>
auto Singleton<Class>::Instance() -> Class&
{
	if (_instance == nullptr)
	{
		std::cerr << typeid(Class).name() << " was not instantiated\n";
		std::abort();
	}
	return *_instance;
}

template <class Class>
Class* Singleton<Class>::_instance = nullptr;
}
