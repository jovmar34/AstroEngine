#include "aspch.h"
#include "LayerStack.h"

Astro::LayerStack::LayerStack()
{
}

Astro::LayerStack::~LayerStack()
{
	for (Layer* layer : m_Layers)
		delete layer;
}

void Astro::LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
	m_LayerInsertIndex++;
}

void Astro::LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

void Astro::LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}
}

void Astro::LayerStack::PopOverlay(Layer* overlay)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it != m_Layers.end())
	{
		m_Layers.erase(it);
	}
}
