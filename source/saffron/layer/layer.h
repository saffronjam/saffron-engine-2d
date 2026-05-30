#pragma once

#include <SFML/Window/Event.hpp>

#include "saffron/base.h"
#include "saffron/core/batch.h"

namespace saffron
{
struct Layer
{
	virtual ~Layer() = default;

	virtual void OnAttach(std::shared_ptr<Batch>& loader) = 0;
	virtual void OnDetach() = 0;
	virtual void OnPreFrame() = 0;
	virtual void OnPostFrame() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnGuiRender() = 0;
	virtual void OnEvent(const sf::Event& event) = 0;
	virtual auto Name() const -> const std::string& = 0;
};

class LayerImpl : public Layer
{
public:
	explicit LayerImpl(const std::string& name);
	~LayerImpl() override = default;

	void OnAttach(std::shared_ptr<Batch>& loader) override
	{
	}

	void OnDetach() override
	{
	}

	void OnPreFrame() override
	{
	}

	void OnPostFrame() override
	{
	}

	void OnUpdate() override
	{
	}

	void OnGuiRender() override
	{
	}

	void OnEvent(const sf::Event& event) override
	{
	}

	auto Name() const -> const std::string& override;

protected:
	std::string _debugName;
};
}
