#include "SaffronPCH.h"

#include "Gui.h"

#include "Saffron/Core/App.h"
#include "Saffron/Core/Global.h"
#include "Saffron/Gui/FadePane.h"

namespace Se
{
FadePane::FadePane(FadeType type, sf::Time duration, sf::Time delay, bool startOnCreation, sf::Color color) :
	FadePane(type, duration, [this](sf::Time timer, sf::Time duration) -> Uint8
	{
		return DefaultAlphaFunction(timer, duration);
	}, delay, startOnCreation, color)
{
}

FadePane::FadePane(FadeType type, sf::Time duration, FadeFn alphaFunction, sf::Time delay, bool startOnCreation,
                   sf::Color color) :
	_type(type),
	_alphaFunction(Move(alphaFunction)),
	_wantFade(startOnCreation),
	_fadeColor(color),
	_timer(sf::Time::Zero),
	_duration(duration),
	_delay(delay)
{
}

void FadePane::OnUpdate()
{
	if (_wantFade)
	{
		const auto dt = Global::Clock::FrameTime();
		if (_delayTimer >= _delay)
		{
			if (_timer >= _duration)
			{
				_timer = _duration;
				_wantFade = false;
				Finished.Invoke();
			}
			else
			{
				_timer += dt;
			}
		}
		else
		{
			_delayTimer += dt;
		}
	}
}

void FadePane::OnGuiRender() const
{
	const auto alpha = _alphaFunction(_timer, _duration);
	const auto color = IM_COL32(0, 0, 0, alpha);
	const ImVec2 position(0.0f, 0.0f);
	const auto size = App::Instance().Window().Size();
	const ImVec2 imSize = {static_cast<float>(size.x), static_cast<float>(size.y)};
	ImGui::GetOverlayDrawList()->AddRectFilled(position, imSize, color);
}

void FadePane::Start()
{
	_wantFade = true;
}

auto FadePane::IsActive() const -> bool { return _wantFade; }

auto FadePane::DefaultAlphaFunction(sf::Time timer, sf::Time duration) const -> Uint8
{
	if (_type == FadeType::In)
	{
		return 255 - _timer / _duration * 255.0f;
	}
	return _timer / _duration * 255.0f;
}
}
