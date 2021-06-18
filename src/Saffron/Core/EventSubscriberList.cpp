#include "SaffronPCH.h"

#include "Saffron/Core/EventSubscriberList.h"

namespace Se
{
void EventSubscriberList<void>::Invoke()
{
	for (const auto& [token, subscriber] : _subscribers)
	{
		if (subscriber())
		{
			break;
		}
	}
}

auto EventSubscriberList<void>::Subscribe(Handler handler) -> CancellationToken
{
	const CancellationToken token;
	_subscribers.emplace(token, Move(handler));
	return token;
}

void EventSubscriberList<void>::Unsubscribe(CancellationToken token)
{
	_subscribers.erase(token);
}

auto EventSubscriberList<void>::Empty() const -> bool
{
	return _subscribers.empty();
}

auto EventSubscriberList<void>::operator+=(Handler handler) -> CancellationToken
{
	return Subscribe(handler);
}

void EventSubscriberList<void>::operator-=(CancellationToken token)
{
	Unsubscribe(token);
}
}
