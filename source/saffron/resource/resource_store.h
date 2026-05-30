#pragma once

#include "saffron/base.h"

namespace saffron
{
template <class ItemType>
class ResourceStore : public Singleton<ResourceStore<ItemType>>
{
public:
	ResourceStore() :
		Singleton<ResourceStore<ItemType>>(this)
	{
	}

	auto TryFetch(const std::filesystem::path& path, bool copy) -> Result<std::shared_ptr<ItemType>>;
	auto Fetch(const std::filesystem::path& path, bool copy) -> std::shared_ptr<ItemType>;

	auto Persist(const std::filesystem::path& path, const std::shared_ptr<ItemType>& resource)
	{
		_resources.emplace(path.string(), resource);
		_persistent.emplace(path.string(), resource);
	}

protected:
	virtual auto TryLoad(std::filesystem::path path) -> Result<std::shared_ptr<ItemType>>;
	virtual auto TryCopy(const std::shared_ptr<ItemType>& value) -> Result<std::shared_ptr<ItemType>>;
	virtual auto Location() -> std::filesystem::path;

private:
	std::unordered_map<std::string, std::weak_ptr<ItemType>> _resources;
	std::unordered_map<std::string, std::shared_ptr<ItemType>> _persistent;
};


template <class StoreType>
auto ResourceStore<StoreType>::TryFetch(const std::filesystem::path& path, bool copy) -> Result<std::shared_ptr<StoreType>>
{
	std::string newPath(Location().string() + path.string());
	std::shared_ptr<StoreType> newResource;
	if (!_resources.contains(newPath))
	{
		auto loaded = TryLoad(newPath);
		if (!loaded)
		{
			return std::unexpected(loaded.error());
		}
		newResource = *loaded;
		_resources.emplace(newPath, newResource);
	}
	else
	{
		newResource = _resources.at(newPath).lock();
		if (newResource == nullptr)
		{
			auto loaded = TryLoad(newPath);
			if (!loaded)
			{
				return std::unexpected(loaded.error());
			}
			newResource = *loaded;
			_resources[newPath] = newResource;
		}
	}

	if (copy)
	{
		return TryCopy(newResource);
	}
	return newResource;
}

template <class StoreType>
auto ResourceStore<StoreType>::Fetch(const std::filesystem::path& path, bool copy) -> std::shared_ptr<StoreType>
{
	auto result = TryFetch(path, copy);
	if (!result)
	{
		Log::CoreError(result.error().message);
		return nullptr;
	}
	return *result;
}

template <class StoreType>
auto ResourceStore<StoreType>::TryLoad(std::filesystem::path path) -> Result<std::shared_ptr<StoreType>>
{
	return std::unexpected(Error{ErrorCode::Resource, "Load was called on store that did not implement it: " + path.string()});
}

template <class StoreType>
auto ResourceStore<StoreType>::TryCopy(const std::shared_ptr<StoreType>& value) -> Result<std::shared_ptr<StoreType>>
{
	return std::unexpected(Error{ErrorCode::Resource, "Copy was called on store that did not implement it"});
}

template <class StoreType>
auto ResourceStore<StoreType>::Location() -> std::filesystem::path
{
	Log::CoreError("GetLocation was called on store that did not implement it");
	return {};
}
}
