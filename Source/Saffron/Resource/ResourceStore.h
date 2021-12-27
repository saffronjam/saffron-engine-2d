#pragma once

#include "Saffron/Base.h"

namespace Se
{
template <class ItemType>
class ResourceStore : public Singleton<ResourceStore<ItemType>>
{
public:
	ResourceStore() :
		Singleton<ResourceStore<ItemType>>(this)
	{
	}

	auto Fetch(const std::filesystem::path& path, bool copy) -> std::shared_ptr<ItemType>;

	auto Persist(const std::filesystem::path& path, const std::shared_ptr<ItemType>& resource)
	{
		_resources.emplace(path.string(), resource);
		_persistent.emplace(path.string(), resource);
	}

protected:
	virtual auto Load(std::filesystem::path path) -> std::shared_ptr<ItemType>;
	virtual auto Copy(const std::shared_ptr<ItemType>& value) -> std::shared_ptr<ItemType>;
	virtual auto Location() -> std::filesystem::path;

private:
	std::unordered_map<std::string, std::weak_ptr<ItemType>> _resources;
	std::unordered_map<std::string, std::shared_ptr<ItemType>> _persistent;
};


template <class StoreType>
auto ResourceStore<StoreType>::Fetch(const std::filesystem::path& path, bool copy) -> std::shared_ptr<StoreType>
{
	std::string newPath(Location().string() + path.string());
	std::shared_ptr<StoreType> newResoure;
	if (!_resources.contains(newPath))
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
auto ResourceStore<StoreType>::Load(std::filesystem::path path) -> std::shared_ptr<StoreType>
{
	Debug::Break("Load was called on store that did not implement it");
	return nullptr;
}

template <class StoreType>
auto ResourceStore<StoreType>::Copy(const std::shared_ptr<StoreType>& value) -> std::shared_ptr<StoreType>
{
	Debug::Break("Copy was called on store that did not implement it");
	return nullptr;
}

template <class StoreType>
auto ResourceStore<StoreType>::Location() -> std::filesystem::path
{
	Debug::Break("GetLocation was called on store that did not implement it");
	return {};
}
}
