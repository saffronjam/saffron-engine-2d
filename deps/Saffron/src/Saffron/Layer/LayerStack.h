#pragma once

#include "Saffron/Config.h"
#include "Layer.h"

namespace Se
{
class LayerStack : public Signaller
{
public:
	struct Signals
	{
		static SignalAggregate<std::shared_ptr<Layer>> OnPushLayer;
		static SignalAggregate<std::shared_ptr<Layer>> OnPushOverlay;
		static SignalAggregate<std::shared_ptr<Layer>> OnPopLayer;
		static SignalAggregate<std::shared_ptr<Layer>> OnPopOverlay;
	};

public:
	LayerStack() = default;
	~LayerStack();

	void PushLayer(std::shared_ptr<Layer>, std::shared_ptr<BatchLoader>& batchLoader);
	void PushOverlay(std::shared_ptr<Layer>, std::shared_ptr<BatchLoader>& batchLoader);
	void PopLayer(int count);
	void PopOverlay(int count);
	void EraseLayer(std::shared_ptr<Layer> layer);
	void EraseOverlay(std::shared_ptr<Layer> overlay);

	void Clear();
	auto Front() -> std::shared_ptr<Layer>;
	auto Back() -> std::shared_ptr<Layer>;

	auto begin() -> List<std::shared_ptr<Layer>>::iterator { return _layers.begin(); }

	auto end() -> List<std::shared_ptr<Layer>>::iterator { return _layers.end(); }

private:
	List<std::shared_ptr<Layer>> _layers;
	unsigned int _layerInsertIndex = 0;
};
}
