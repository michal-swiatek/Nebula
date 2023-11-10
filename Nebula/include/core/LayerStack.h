//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_LAYERSTACK_H
#define NEBULAENGINE_LAYERSTACK_H

#include <vector>

#include "Layer.h"
#include "Types.h"

namespace nebula {

    class NEBULA_API LayerStack
    {
    public:
        using LayerID = uint32_t;
        using Container = std::vector<Scope<Layer>>;

        LayerStack() = default;
        ~LayerStack();

        LayerID pushLayer(Layer* layer);
        LayerID pushOverlay(Layer* layer);
        Scope<Layer> popLayer(LayerID layer_id);
        Scope<Layer> popOverlay(LayerID layer_id);
        View<Layer> getLayer(LayerID layer_id);
        View<Layer> getOverlay(LayerID layer_id);

        [[nodiscard]] Container::iterator begin()                      { return m_layers.begin(); }
        [[nodiscard]] Container::iterator end()                        { return m_layers.end(); }
        [[nodiscard]] Container::reverse_iterator rbegin()             { return m_layers.rbegin(); }
        [[nodiscard]] Container::reverse_iterator rend()               { return m_layers.rend(); }

        [[nodiscard]] Container::const_iterator begin() const          { return m_layers.begin(); }
        [[nodiscard]] Container::const_iterator end()	const           { return m_layers.end(); }
        [[nodiscard]] Container::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
        [[nodiscard]] Container::const_reverse_iterator rend() const   { return m_layers.rend(); }

    private:
        Container m_layers;

        int m_layer_insert_index = 0;
        static LayerID s_layer_counter;
    };

}

#endif //NEBULAENGINE_LAYERSTACK_H
