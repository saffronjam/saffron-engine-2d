#pragma once

#include <SFML/Window/Event.hpp>

#include "Saffron/Base.h"
#include "Saffron/Core/Batch.h"

namespace Se
{
class Layer
{
public:
	explicit Layer(const String& name = "Layer");
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

	auto Name() const -> const String&;

protected:
	String _debugName;
};
}
