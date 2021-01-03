#include "SaffronPCH.h"

#include "Saffron/Core/Global.h"
#include "Saffron/Gui/Gui.h"
#include "Saffron/Gui/SplashScreenPane.h"
#include "Saffron/Resource/TextureStore.h"

namespace Se
{
SplashScreenPane::SplashScreenPane(const std::shared_ptr<BatchLoader> &batchLoader)
	: _batchLoader(batchLoader),
	_texture(TextureStore::GetCopy("res/Editor/Saffron.png")),
	_finalizingStatus("Finalizing")
{
}

void SplashScreenPane::OnUpdate()
{
	if ( _hidden )
		return;

	if ( std::abs(_batchLoader->GetProgress() - _goalProgressView) > 0.1f )
	{
		_goalProgressView = _batchLoader->GetProgress();
		_currentSinTimer = 0.0f;
	}
	else
	{
		_currentSinTimer += Global::Clock::GetFrameTime().asSeconds();
	}
	_currentProgressView += (_goalProgressView - _currentProgressView) * std::sin(_currentSinTimer / (2.0f * Math::PI));
}

void SplashScreenPane::OnGuiRender()
{
	if ( _hidden )
		return;

	ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);

	OutputStringStream oss;
	oss << "Loading Screen##";
	ImGui::Begin(oss.str().c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	ImGui::GetWindowDrawList()->AddRectFilled({ 0.0f, 0.0f }, ImGui::GetWindowSize(), IM_COL32(0, 0, 0, 255));

	const auto windowSize = ImGui::GetWindowSize();

	const auto logoWidth = 200;
	const auto logoHeight = 200;
	ImGui::SetCursorPos({ windowSize.x / 2.0f - logoWidth / 2.0f, 2.0f * windowSize.y / 5.0f - logoHeight / 2.0f });
	ImGui::Image(reinterpret_cast<ImTextureID>(_texture.getNativeHandle()), { logoWidth, logoHeight }, { 0.0f, 0.0f }, { 1.0f, 1.0f });
	//Gui::Image(_texture, , sf::FloatRect{ 0.0f, 0.0f, 1.0f, 1.0f }, sf::Color(255, 255, 255, 255));

	Gui::SetFontSize(36);
	ImGui::NewLine();
	Gui::SetFontSize(48);

	const char *title = "Saffron Engine";
	const float titleTextWidth = ImGui::CalcTextSize(title).x;
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - titleTextWidth / 2.0f);
	ImGui::Text(title);

	/*IM_COL32(106, 58, 206, 255),
	IM_COL32(106, 58, 206, 255),
	IM_COL32(176, 121, 220, 255),
	IM_COL32(176, 121, 220, 255));*/
	Gui::SetFontSize(24);

	oss.str("");
	oss.clear();

	oss << std::setprecision(0) << std::fixed << _currentProgressView << "%";

	ImGui::NewLine();
	const float currentProgressTextWidth = ImGui::CalcTextSize(oss.str().c_str()).x;
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - currentProgressTextWidth / 2.0f);
	ImGui::Text("%s", oss.str().c_str());

	Gui::SetFontSize(18);

	const String *status = _goalProgressView < 100.0f ? _batchLoader->GetStatus() : &_finalizingStatus;
	if ( status && !status->empty() )
	{
		const auto infoTextWidth = ImGui::CalcTextSize(status->c_str()).x;
		ImGui::NewLine();
		ImGui::SetCursorPosX(windowSize.x / 2.0f - infoTextWidth / 2.0f);
		ImGui::Text("%s", status->c_str());
	}

	ImGui::End();
}

void SplashScreenPane::Show()
{
	_hidden = false;
}

void SplashScreenPane::Hide()
{
	_hidden = true;
}

bool SplashScreenPane::IsIdle() const
{
	return  static_cast<int>(std::round(_currentProgressView)) == static_cast<int>(std::round(_goalProgressView));
}

bool SplashScreenPane::IsFinished() const
{
	return _currentProgressView > 99.9f;
}
}
