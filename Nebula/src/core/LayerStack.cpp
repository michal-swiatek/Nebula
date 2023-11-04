//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#include "core/LayerStack.h"

namespace nebula {

    LayerStack::LayerID LayerStack::s_layer_counter = 0;

    LayerStack::~LayerStack()
    {
        for (auto& layer : m_layers)
            layer->onDetach();
        m_layers.clear();
    }

    LayerStack::LayerID LayerStack::pushLayer(Layer* layer)
    {
        layer->m_id = s_layer_counter++;
        m_layers.emplace(m_layers.begin() + m_layer_insert_index++, layer);
        return layer->getID();
    }

    LayerStack::LayerID LayerStack::pushOverlay(Layer* layer)
    {
        layer->m_id = s_layer_counter++;
        m_layers.emplace_back(layer);
        return layer->getID();
    }

    Scope<Layer> LayerStack::popLayer(LayerStack::LayerID layer_id)
    {
        auto it = std::find_if(
                m_layers.begin(),
                m_layers.begin() + m_layer_insert_index,
                [layer_id](const auto& layer) { return layer->getID() == layer_id; }
        );

        if (it != m_layers.begin() + m_layer_insert_index)
        {
            auto layer = std::move(*it);
            m_layers.erase(it);

            layer->onDetach();
            m_layer_insert_index--;

            return layer;
        }

        return nullptr;
    }

    Scope<Layer> LayerStack::popOverlay(LayerStack::LayerID layer_id)
    {
        auto it = std::find_if(
                m_layers.begin() + m_layer_insert_index,
                m_layers.end(),
                [layer_id](const auto& layer) { return layer->getID() == layer_id; }
        );

        if (it != m_layers.end())
        {
            auto layer = std::move(*it);
            m_layers.erase(it);

            layer->onDetach();
            m_layer_insert_index--;

            return layer;
        }

        return nullptr;
    }

}
