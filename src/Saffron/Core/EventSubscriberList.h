#pragma once

#include "Saffron/Core/UUID.h"
#include "Saffron/Core/TypeDefs.h"

namespace Se
{
template <typename EventArgs>
class EventSubscriberList
{
public:
	using CancellationToken = UUID;
	using Handler = Function<bool(EventArgs)>;

public:
	void Invoke(EventArgs args);

	auto Subscribe(Handler handler) -> CancellationToken;
	void Unsubscribe(CancellationToken token);
	auto Empty() -> bool;

	auto operator +=(Handler handler) -> CancellationToken;
	void operator -=(CancellationToken token);

private:
	HashMap<CancellationToken, Handler> _subscribers;
};

template <>
class EventSubscriberList<void>
{
public:
	using CancellationToken = UUID;
	using Handler = Function<bool()>;

public:
	void Invoke();

	auto Subscribe(Handler handler) -> CancellationToken;
	void Unsubscribe(CancellationToken token);
	auto Empty() const -> bool;

	auto operator +=(Handler handler) -> CancellationToken;
	void operator -=(CancellationToken token);

private:
	HashMap<CancellationToken, Handler> _subscribers;
};

template <typename EventArgs>
void EventSubscriberList<EventArgs>::Invoke(EventArgs args)
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
auto EventSubscriberList<EventArgs>::Subscribe(Handler handler) -> CancellationToken
{
	const CancellationToken token;
	_subscribers.emplace(token, Move(handler));
	return token;
}

template <typename EventArgs>
void EventSubscriberList<EventArgs>::Unsubscribe(CancellationToken token)
{
	_subscribers.erase(token);
}

template <typename EventArgs>
auto EventSubscriberList<EventArgs>::Empty() -> bool
{
	return _subscribers.empty();
}

template <typename EventArgs>
auto EventSubscriberList<EventArgs>::operator+=(Handler handler) -> CancellationToken
{
	return Subscribe(handler);
}

template <typename EventArgs>
void EventSubscriberList<EventArgs>::operator-=(CancellationToken token)
{
	Unsubscribe(token);
}
}
