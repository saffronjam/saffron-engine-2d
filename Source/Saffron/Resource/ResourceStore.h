#pragma once

#include "Saffron/Base.h"

namespace Se
{
template <class ItemType>
class ResourceStore : public SingleTon<ResourceStore<ItemType>>
{
public:
	ResourceStore() :
		SingleTon<ResourceStore<ItemType>>(this)
	{
	}

	auto Fetch(const Path& Path, bool copy) -> Shared<ItemType>;

	auto Persist(const Path& path, const Shared<ItemType>& resource)
	{
		_resources.emplace(path.string(), resource);
		_persistent.emplace(path.string(), resource);
	}

protected:
	virtual auto Load(Path Path) -> Shared<ItemType>;
	virtual auto Copy(const Shared<ItemType>& value) -> Shared<ItemType>;
	virtual auto Location() -> Path;

private:
	HashMap<String, Weak<ItemType>> _resources;
	HashMap<String, Shared<ItemType>> _persistent;
};


template <class StoreType>
auto ResourceStore<StoreType>::Fetch(const Path& Path, bool copy) -> Shared<StoreType>
{
	String newPath(Location().string() + Path.string());
	Shared<StoreType> newResoure;
	if (_resources.find(newPath) == _resources.end())
	{
		newResoure = Load(newPath);
		_resources.emplace(newPath, newResoure);
	}
	else
	{
		newResoure = _resources.at(newPath).lock();
	}

	if (copy)
	{
		return Copy(newResoure);
	}
	return newResoure;
}

template <class StoreType>
Shared<StoreType> ResourceStore<StoreType>::Load(Path Path)
{
	Debug::Break("Load was called on store that did not implement it");
	return nullptr;
}

template <class StoreType>
Shared<StoreType> ResourceStore<StoreType>::Copy(const Shared<StoreType>& value)
{
	Debug::Break("Copy was called on store that did not implement it");
	return nullptr;
}

template <class StoreType>
auto ResourceStore<StoreType>::Location() -> Path
{
	Debug::Break("GetLocation was called on store that did not implement it");
	return {};
}
}
