#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "Saffron/Base.h"
#include "Saffron/Core/BatchLoader.h"

namespace Se
{
class SplashScreenPane
{
public:
	explicit SplashScreenPane(const std::shared_ptr<BatchLoader> &batchLoader);

	void OnUpdate();
	void OnGuiRender();

	const std::shared_ptr<BatchLoader> &GetBatchLoader() const { return _batchLoader; }

	void Show();
	void Hide();
	bool IsIdle() const;
	bool IsFinished() const;

private:
	std::shared_ptr<BatchLoader> _batchLoader;
	sf::Texture _texture;
	bool _hidden = false;
	String _finalizingStatus;

	float _goalProgressView = 0.0f;
	float _currentProgressView = 0.0f;
	float _currentSinTimer = 0.0f;
};
}

