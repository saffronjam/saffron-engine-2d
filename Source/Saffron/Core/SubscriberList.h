#pragma once

#include "Saffron/Core/UUID.h"
#include "Saffron/Core/TypeDefs.h"

namespace Se
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

	auto operator +=(Handler handler) -> CancellationToken;
	void operator -=(CancellationToken token);

private:
	std::unordered_map<CancellationToken, Handler> _subscribers;
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

	auto operator +=(Handler handler) -> CancellationToken;
	void operator -=(CancellationToken token);

private:
	std::unordered_map<CancellationToken, Handler> _subscribers;
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
	_subscribers.emplace(token, std::move(handler));
	return token;
}

template <typename EventArgs>
void SubscriberList<EventArgs>::Unsubscribe(CancellationToken token)
{
	_subscribers.erase(token);
}

template <typename EventArgs>
auto SubscriberList<EventArgs>::Empty() -> bool
{
	return _subscribers.empty();
}

template <typename EventArgs>
auto SubscriberList<EventArgs>::operator+=(Handler handler) -> CancellationToken
{
	return Subscribe(handler);
}

template <typename EventArgs>
void SubscriberList<EventArgs>::operator-=(CancellationToken token)
{
	Unsubscribe(token);
}
}
