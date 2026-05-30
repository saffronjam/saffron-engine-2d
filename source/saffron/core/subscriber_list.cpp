#include "saffron_pch.h"

#include "saffron/core/subscriber_list.h"

namespace saffron
{
void SubscriberList<void>::Invoke()
{
	for (const auto& [token, subscriber] : _subscribers)
	{
		if (subscriber())
		{
			break;
		}
	}
}

auto SubscriberList<void>::Subscribe(Handler handler) -> CancellationToken
{
	const CancellationToken token;
	_subscribers.emplace(token.Value(), std::move(handler));
	return token;
}

void SubscriberList<void>::Unsubscribe(CancellationToken token)
{
	_subscribers.erase(token.Value());
}

auto SubscriberList<void>::Empty() const -> bool
{
	return _subscribers.empty();
}

}
