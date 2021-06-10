#include "SaffronPCH.h"

#include "Saffron/Core/Global.h"
#include "Saffron/Gui/Gui.h"
#include "Saffron/Gui/SplashScreenPane.h"
#include "Saffron/Resource/TextureStore.h"

namespace Se
{
SplashScreenPane::SplashScreenPane(const std::shared_ptr<class BatchLoader>& batchLoader) :
	_batchLoader(batchLoader),
	_texture(TextureStore::Get("Editor/Saffron.png", true)),
	_finalizingStatus("Finalizing"),
	_fadeIn(FadeType::In, sf::seconds(0.4f), sf::seconds(0.5f), true),
	_fadeOut(FadeType::Out, sf::seconds(0.4f), [](sf::Time timer, sf::Time duration)
	{
		return std::min(duration, timer * 2.0f) / duration * 255.0f;
	})
{
	_fadeOut.Finished += [this]
	{
		_finished = true;
		return false;
	};
}

void SplashScreenPane::OnUpdate()
{
	if (_hidden) return;

	const auto dt = Global::Clock::FrameTime();

	_fadeIn.OnUpdate();

	if (_fadeOut.IsActive() && !_fadeIn.IsActive())
	{
		if (_holdTimer >= _holdTimerFinished)
		{
			_fadeOut.OnUpdate();
		}
		else
		{
			_holdTimer += dt;
		}
	}

	if (_batchLoader->Progress() > 99.9f)
	{
		_fadeOut.Start();
	}

	if (std::abs(_batchLoader->Progress() - _progressViewFinished) > 0.1f)
	{
		_progressViewFinished = _batchLoader->Progress();
		_progressTimer = sf::Time::Zero;
	}
	else
	{
		_progressTimer += dt;
	}

	_progressView += (_progressViewFinished - _progressView) * std::sin(_progressTimer.asSeconds() / (2.0f * Math::PI));
}

void SplashScreenPane::OnGuiRender()
{
	if (_hidden) return;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	OStringStream oss;
	oss << "Loading Screen##";
	ImGui::Begin(oss.str().c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);


	ImGui::GetWindowDrawList()->AddRectFilled({0.0f, 0.0f}, ImGui::GetWindowSize(), IM_COL32(0, 0, 0, 255));

	const auto windowSize = ImGui::GetWindowSize();

	const auto logoWidth = 200;
	const auto logoHeight = 200;
	ImGui::SetCursorPos({windowSize.x / 2.0f - logoWidth / 2.0f, 2.0f * windowSize.y / 5.0f - logoHeight / 2.0f});
	Gui::Image(*_texture, {logoWidth, logoHeight}, sf::FloatRect{0.0f, 0.0f, 1.0f, 1.0f}, sf::Color(255, 255, 255, 255));

	Gui::SetFontSize(36);
	ImGui::NewLine();
	Gui::SetFontSize(48);

	const char* title = _title.c_str();
	const float titleTextWidth = ImGui::CalcTextSize(title).x;
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - titleTextWidth / 2.0f);
	ImGui::Text("%s", title);

	/*IM_COL32(106, 58, 206, 255),
	IM_COL32(106, 58, 206, 255),
	IM_COL32(176, 121, 220, 255),
	IM_COL32(176, 121, 220, 255));*/
	Gui::SetFontSize(24);

	oss.str("");
	oss.clear();

	oss << std::setprecision(0) << std::fixed << _progressView << "%";

	ImGui::NewLine();
	const float currentProgressTextWidth = ImGui::CalcTextSize(oss.str().c_str()).x;
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - currentProgressTextWidth / 2.0f);
	ImGui::Text("%s", oss.str().c_str());

	Gui::SetFontSize(18);

	const String* status = _progressViewFinished < 100.0f ? _batchLoader->Status() : &_finalizingStatus;
	if (status && !status->empty())
	{
		const auto infoTextWidth = ImGui::CalcTextSize(status->c_str()).x;
		ImGui::NewLine();
		ImGui::SetCursorPosX(windowSize.x / 2.0f - infoTextWidth / 2.0f);
		ImGui::Text("%s", status->c_str());
	}

	_fadeIn.OnGuiRender();
	_fadeOut.OnGuiRender();

	ImGui::End();
}

auto SplashScreenPane::BatchLoader() const -> const std::shared_ptr<Se::BatchLoader>& { return _batchLoader; }

void SplashScreenPane::Show()
{
	_hidden = false;
}

void SplashScreenPane::Hide()
{
	_hidden = true;
}

auto SplashScreenPane::Idle() const -> bool
{
	return static_cast<int>(std::round(_progressView)) == static_cast<int>(std::round(_progressViewFinished));
}

auto SplashScreenPane::Finished() const -> bool
{
	return _finished;
}

auto SplashScreenPane::Hidden() const -> bool
{
	return _hidden;
}
}
