#pragma once

#include <complex>
#include <SFML/System/Vector2.hpp>

#include "saffron/base.h"
#include "saffron/math/saffron_math.h"

namespace saffron
{
class ComputeShader
{
public:
	void Bind();
	void Unbind();

	void Dispatch(uint xGroup, uint yGroup, uint zGroup);

	auto TryLoadFromFile(const std::filesystem::path& filepath) -> Status;
	auto LoadFromFile(const std::filesystem::path& filepath) -> bool;

	auto TrySetFloat(const std::string& name, float value) -> Status;
	auto TrySetDouble(const std::string& name, double value) -> Status;
	auto TrySetInt(const std::string& name, int value) -> Status;
	void SetFloat(const std::string& name, float value);
	void SetDouble(const std::string& name, double value);
	void SetInt(const std::string& name, int value);;

	void SetVector2d(const std::string& name, const sf::Vector2<double>& value);
	void SetVector2d(const std::string& name, const std::complex<double>& value);
	void SetVector4f(const std::string& name, const sf::Vector4f& value);

	static void AwaitFinish();

private:
	void BindThenUnbind(const std::function<void()>& fn);

private:
	uint _id{};
};
}
