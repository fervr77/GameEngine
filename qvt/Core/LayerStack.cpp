#include "LayerStack.h"
#include <algorithm>


namespace qvt
{
	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(begin() + m_LayerInsertIndex, layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		m_Layers.emplace_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(begin(), begin() + m_LayerInsertIndex, layer);
		if (it != (begin() + m_LayerInsertIndex))
		{
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(begin() + m_LayerInsertIndex, end(), overlay);
		if (it != end())
		{
			m_Layers.erase(it);
		}
	}


}
