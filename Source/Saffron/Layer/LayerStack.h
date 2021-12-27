#pragma once

#include "Saffron/Base.h"
#include "Saffron/Layer/Layer.h"

namespace Se
{
class LayerStack
{
public:
	LayerStack() = default;
	~LayerStack();

	void PushLayer(std::shared_ptr<Layer>, std::shared_ptr<Batch>& batchLoader);
	void PushOverlay(std::shared_ptr<Layer>, std::shared_ptr<Batch>& batchLoader);
	void PopLayer(int count);
	void PopOverlay(int count);
	void EraseLayer(std::shared_ptr<Layer> layer);
	void EraseOverlay(std::shared_ptr<Layer> overlay);

	void Clear();
	auto Front() -> std::shared_ptr<Layer>;
	auto Back() -> std::shared_ptr<Layer>;

	auto begin() -> std::vector<std::shared_ptr<Layer>>::iterator;
	auto end() -> std::vector<std::shared_ptr<Layer>>::iterator;

public:
	SubscriberList<const std::shared_ptr<Layer>&> OnPushLayer;
	SubscriberList<const std::shared_ptr<Layer>&> OnPushOverlay;
	SubscriberList<const std::shared_ptr<Layer>&> OnPopLayer;
	SubscriberList<const std::shared_ptr<Layer>&> OnPopOverlay;

private:
	std::vector<std::shared_ptr<Layer>> _layers;
	unsigned int _layerInsertIndex = 0;
};
}
