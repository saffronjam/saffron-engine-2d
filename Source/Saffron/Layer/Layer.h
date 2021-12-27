#pragma once

#include <SFML/Window/Event.hpp>

#include "Saffron/Base.h"
#include "Saffron/Core/Batch.h"

namespace Se
{
class Layer
{
public:
	explicit Layer(const std::string& name = "Layer");
	virtual ~Layer() = default;

	virtual void OnAttach(std::shared_ptr<Batch>& loader)
	{
	}

	virtual void OnDetach()
	{
	}

	virtual void OnPreFrame()
	{
	}

	virtual void OnPostFrame()
	{
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnGuiRender()
	{
	}

	virtual void OnEvent(const sf::Event& event)
	{
	}

	auto Name() const -> const std::string&;

protected:
	std::string _debugName;
};
}
