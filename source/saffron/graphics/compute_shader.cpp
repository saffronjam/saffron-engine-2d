#include "saffron_pch.h"

#include <glad/glad.h>

#include "saffron/core/filesystem.h"
#include "saffron/graphics/compute_shader.h"

#include <complex>

namespace saffron
{
void ComputeShader::Bind()
{
	glUseProgram(_id);
}

void ComputeShader::Unbind()
{
	glUseProgram(0);
}

void ComputeShader::Dispatch(uint xGroup, uint yGroup, uint zGroup)
{
	BindThenUnbind([&]
	{
		glDispatchCompute(xGroup, yGroup, zGroup);
	});
}

auto ComputeShader::TryLoadFromFile(const std::filesystem::path& filepath) -> Status
{
	auto exists = Filesystem::TryFileExists(filepath);
	if (!exists)
	{
		return std::unexpected(exists.error());
	}
	if (!*exists)
	{
		return std::unexpected(Error{ErrorCode::Io, "Compute shader file does not exist: " + filepath.string()});
	}

	std::string content;
	std::fstream fileStream(filepath, std::ios::in);

	if (!fileStream.is_open())
	{
		return std::unexpected(Error{ErrorCode::Io, "Could not read compute shader file " + filepath.string()});
	}

	std::string line;
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();

	const auto* contentCStr = content.c_str();
	const auto shader = glCreateShader(GL_COMPUTE_SHADER);
	if (shader < 1)
	{
		return std::unexpected(Error{ErrorCode::Graphics, "Failed to create compute shader object for " + filepath.string()});
	}

	glShaderSource(shader, 1, &contentCStr, nullptr);
	// Try compile
	{
		glCompileShader(shader);
		int status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == 0)
		{
			return std::unexpected(Error{ErrorCode::Graphics, "Failed to compile compute shader " + filepath.string()});
		}
	}

	_id = glCreateProgram();
	if (_id < 1)
	{
		return std::unexpected(Error{ErrorCode::Graphics, "Failed to create compute shader program for " + filepath.string()});
	}
	glAttachShader(_id, shader);
	// Try link
	{
		glLinkProgram(_id);
		int status;
		glGetShaderiv(shader, GL_LINK_STATUS, &status);
		if (status == 0)
		{
			_id = 0;
			return std::unexpected(Error{ErrorCode::Graphics, "Failed to link compute shader " + filepath.string()});
		}
	}

	return {};
}

auto ComputeShader::LoadFromFile(const std::filesystem::path& filepath) -> bool
{
	auto result = TryLoadFromFile(filepath);
	if (!result)
	{
		Log::CoreError(result.error().message);
		return false;
	}
	return true;
}

auto ComputeShader::TrySetFloat(const std::string& name, float value) -> Status
{
	BindThenUnbind([&]
	{
		const auto loc = glGetUniformLocation(_id, name.c_str());
		if (loc == -1)
		{
			return;
		}
		glUniform1f(loc, value);
	});
	if (glGetUniformLocation(_id, name.c_str()) == -1)
	{
		return std::unexpected(Error{ErrorCode::Graphics, "Missing compute shader float uniform: " + name});
	}
	return {};
}

void ComputeShader::SetFloat(const std::string& name, float value)
{
	auto result = TrySetFloat(name, value);
	if (!result)
	{
		Log::CoreError(result.error().message);
	}
}

auto ComputeShader::TrySetDouble(const std::string& name, double value) -> Status
{
	BindThenUnbind([&]
	{
		const auto loc = glGetUniformLocation(_id, name.c_str());
		if (loc == -1)
		{
			return;
		}
		glUniform1d(loc, value);
	});
	if (glGetUniformLocation(_id, name.c_str()) == -1)
	{
		return std::unexpected(Error{ErrorCode::Graphics, "Missing compute shader double uniform: " + name});
	}
	return {};
}

void ComputeShader::SetDouble(const std::string& name, double value)
{
	auto result = TrySetDouble(name, value);
	if (!result)
	{
		Log::CoreError(result.error().message);
	}
}

auto ComputeShader::TrySetInt(const std::string& name, int value) -> Status
{
	BindThenUnbind([&]
	{
		const auto loc = glGetUniformLocation(_id, name.c_str());
		if (loc == -1)
		{
			return;
		}
		glUniform1i(loc, value);
	});
	if (glGetUniformLocation(_id, name.c_str()) == -1)
	{
		return std::unexpected(Error{ErrorCode::Graphics, "Missing compute shader int uniform: " + name});
	}
	return {};
}

void ComputeShader::SetInt(const std::string& name, int value)
{
	auto result = TrySetInt(name, value);
	if (!result)
	{
		Log::CoreError(result.error().message);
	}
}

void ComputeShader::SetVector2d(const std::string& name, const sf::Vector2<double>& value)
{
	BindThenUnbind([&]
	{
		const auto loc = glGetUniformLocation(_id, name.c_str());
		if (loc == -1)
		{
			Log::CoreError("Missing compute shader vec2 uniform: {0}", name);
			return;
		}
		glUniform2d(loc, value.x, value.y);
	});
}

void ComputeShader::SetVector2d(const std::string& name, const std::complex<double>& value)
{
	SetVector2d(name, sf::Vector2<double>(value.real(), value.imag()));
}

void ComputeShader::SetVector4f(const std::string& name, const sf::Vector4f& value)
{
	BindThenUnbind([&]
	{
		const auto loc = glGetUniformLocation(_id, name.c_str());
		if (loc == -1)
		{
			Log::CoreError("Missing compute shader vec4 uniform: {0}", name);
			return;
		}
		glUniform4f(loc, value.x, value.y, value.z, value.w);
	});
}

void ComputeShader::AwaitFinish()
{
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void ComputeShader::BindThenUnbind(const std::function<void()>& fn)
{
	Bind();
	fn();
	Unbind();
}
}
