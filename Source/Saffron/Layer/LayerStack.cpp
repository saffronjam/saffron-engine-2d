#include "SaffronPCH.h"

#include "Saffron/Layer/LayerStack.h"

namespace Se
{
LayerStack::~LayerStack()
{
	for (const auto& layer : _layers)
	{
		layer->OnDetach();
	}
	_layers.clear();
}

void LayerStack::PushLayer(std::shared_ptr<Layer> layer, std::shared_ptr<Batch>& batchLoader)
{
	const auto& newLayer = *_layers.emplace(_layers.begin() + _layerInsertIndex, layer);
	_layerInsertIndex++;
	OnPushLayer.Invoke(newLayer);
	newLayer->OnAttach(batchLoader);
}

void LayerStack::PushOverlay(std::shared_ptr<Layer> overlay, std::shared_ptr<Batch>& batchLoader)
{
	// TODO: Implement
	Debug::Assert("NOT IMPLEMETED");
	const auto& newOverlay = _layers.emplace_back(overlay);
	OnPushOverlay.Invoke(newOverlay);
}

void LayerStack::PopLayer(int count)
{
	for (int i = 0; i < count; i++)
	{
		OnPopLayer.Invoke(_layers.back());
		_layers.back()->OnDetach();
		_layers.pop_back();
		_layerInsertIndex--;
	}
}

void LayerStack::PopOverlay(int count)
{
	// TODO: Implement
	Debug::Assert("NOT IMPLEMETED");
	for (int i = 0; i < count; i++)
	{
		OnPopOverlay.Invoke(_layers.back());
		_layers.back()->OnDetach();
		_layers.pop_back();
	}
}

void LayerStack::EraseLayer(std::shared_ptr<Layer> layer)
{
	const auto it = std::find(_layers.begin(), _layers.end(), layer);
	if (it != _layers.end())
	{
		OnPopLayer.Invoke(*it);
		(*it)->OnDetach();
		_layers.erase(it);
		_layerInsertIndex--;
	}
}

void LayerStack::EraseOverlay(std::shared_ptr<Layer> overlay)
{
	// TODO: Implement
	Debug::Assert("NOT IMPLEMETED");
	const auto it = std::find(_layers.begin(), _layers.end(), overlay);
	if (it != _layers.end())
	{
		_layers.erase(it);
	}
}

void LayerStack::Clear()
{
	for (auto& layer : _layers)
	{
		layer->OnDetach();
	}
	_layers.clear();
}

auto LayerStack::Front() -> std::shared_ptr<Layer>
{
	return _layers.front();
}

auto LayerStack::Back() -> std::shared_ptr<Layer>
{
	return _layers.back();
}

auto LayerStack::begin() -> List<std::shared_ptr<Layer>>::iterator { return _layers.begin(); }

auto LayerStack::end() -> List<std::shared_ptr<Layer>>::iterator { return _layers.end(); }
}
