#pragma once

#include <complex>
#include <SFML/System/Vector2.hpp>

#include "Saffron/Base.h"
#include "Saffron/Math/SaffronMath.h"

namespace Se
{
class ComputeShader
{
public:
	void Bind();
	void Unbind();

	void Dispatch(uint xGroup, uint yGroup, uint zGroup);

	bool LoadFromFile(const std::filesystem::path& filepath);

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
