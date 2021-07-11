#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Saffron/Base.h"
#include "Saffron/Core/Batch.h"
#include "Saffron/Gui/FadePane.h"

namespace Se
{
class SplashScreenPane
{
public:
	explicit SplashScreenPane(Shared<class Batch> batch);

	void OnUpdate();
	void OnGuiRender();

	auto BatchLoader() const -> const Shared<class Batch>&;

	void Show();
	void Hide();
	void FadeOut();

	auto FinishedFadeIn() const -> bool;
	auto FinishedFadeOut() const -> bool;
	
	auto Idle() const -> bool;
	auto BatchFinished() const -> bool;
	auto Hidden() const -> bool;
	
private:
	String _title;

	Shared<class Batch> _batch;
	Shared<sf::Texture> _texture;
	bool _hidden = false;
	bool _finishedFadeIn = false;
	bool _finishedFadeOut = false;
	bool _shouldFadeOut = false;

	sf::Time _progressTimer = sf::Time::Zero;
	float _progressView = 0.0f;
	float _progressViewFinished = 0.0f;

	sf::Time _holdTimer = sf::Time::Zero;
	const sf::Time _holdTimerFinished = sf::seconds(0.8f);

	FadePane _fadeIn;
	FadePane _fadeOut;
};
}
