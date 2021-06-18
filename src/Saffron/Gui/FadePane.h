#pragma once

#include <SFML/Graphics/Color.hpp>

#include "Saffron/Base.h"
#include "Saffron/Core/Enums.h"

namespace Se
{
class FadePane
{
public:
	using FadeFn = Function<Uint8(sf::Time, sf::Time)>;


public:
	FadePane(FadeType type, sf::Time duration, sf::Time delay = sf::Time::Zero, bool startOnCreation = false,
	         sf::Color color = sf::Color::Black);
	FadePane(FadeType type, sf::Time duration, FadeFn alphaFunction, sf::Time delay = sf::Time::Zero,
	         bool startOnCreation = false, sf::Color color = sf::Color::Black);

	void OnUpdate();
	void OnGuiRender() const;

	void Start();
	auto IsActive() const -> bool;

public:
	EventSubscriberList<void> Finished;

private:
	auto DefaultAlphaFunction(sf::Time timer, sf::Time duration) const -> Uint8;

private:
	FadeType _type;

	FadeFn _alphaFunction;

	bool _wantFade = false;
	sf::Color _fadeColor;

	sf::Time _timer = sf::Time::Zero;
	const sf::Time _duration;

	sf::Time _delayTimer = sf::Time::Zero;
	const sf::Time _delay;
};
}
