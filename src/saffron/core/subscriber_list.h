#pragma once

#include "saffron/core/uuid.h"
#include "saffron/core/type_defs.h"

namespace saffron
{
template <typename EventArgs>
class SubscriberList
{
public:
	using CancellationToken = UUID;
	using Handler = std::function<bool(EventArgs)>;

public:
	void Invoke(EventArgs args);

	auto Subscribe(Handler handler) -> CancellationToken;
	void Unsubscribe(CancellationToken token);
	auto Empty() -> bool;

private:
	std::unordered_map<ulong, Handler> _subscribers;
};

template <>
class SubscriberList<void>
{
public:
	using CancellationToken = UUID;
	using Handler = std::function<bool()>;

public:
	void Invoke();

	auto Subscribe(Handler handler) -> CancellationToken;
	void Unsubscribe(CancellationToken token);
	auto Empty() const -> bool;

private:
	std::unordered_map<ulong, Handler> _subscribers;
};

template <typename EventArgs>
void SubscriberList<EventArgs>::Invoke(EventArgs args)
{
	for (const auto& [token, subscriber] : _subscribers)
	{
		if (subscriber(args))
		{
			break;
		}
	}
}

template <typename EventArgs>
auto SubscriberList<EventArgs>::Subscribe(Handler handler) -> CancellationToken
{
	const CancellationToken token;
	_subscribers.emplace(token.Value(), std::move(handler));
	return token;
}

template <typename EventArgs>
void SubscriberList<EventArgs>::Unsubscribe(CancellationToken token)
{
	_subscribers.erase(token.Value());
}

template <typename EventArgs>
auto SubscriberList<EventArgs>::Empty() -> bool
{
	return _subscribers.empty();
}

}
