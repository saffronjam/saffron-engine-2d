#include "SaffronPCH.h"

#include "Saffron/Core/SubscriberList.h"

namespace Se
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
	_subscribers.emplace(token, std::move(handler));
	return token;
}

void SubscriberList<void>::Unsubscribe(CancellationToken token)
{
	_subscribers.erase(token);
}

auto SubscriberList<void>::Empty() const -> bool
{
	return _subscribers.empty();
}

auto SubscriberList<void>::operator+=(Handler handler) -> CancellationToken
{
	return Subscribe(handler);
}

void SubscriberList<void>::operator-=(CancellationToken token)
{
	Unsubscribe(token);
}
}
