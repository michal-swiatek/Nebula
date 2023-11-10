//
// Created by michal-swiatek on 04.11.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_LAYER_H
#define NEBULAENGINE_LAYER_H

#include <string>

#include "core/Core.h"
#include "core/Timestep.h"
#include "events/Event.h"

namespace nebula {

    class NEBULA_API Layer
    {
    public:
        explicit Layer(std::string name = "Layer", bool enabled = true) : m_name(std::move(name)), m_enabled(enabled) {}
        virtual ~Layer() = default;

        virtual void onAttach() {}
        virtual void onDetach() {}
        virtual void onUpdate(Timestep delta_time) {}
        virtual void onFixedUpdate(Timestep delta_time) {}
        virtual void onRender() {}
        virtual void onImGuiRender() {}
        virtual void onEvent(Event& event) {}

        [[nodiscard]] const std::string& getName() const { return m_name; }
        [[nodiscard]] uint32_t getID() const { return m_id; }
        [[nodiscard]] bool isEnabled() const { return m_enabled; }

        void setEnabled(bool enabled) { m_enabled = enabled; }

    protected:
        bool m_enabled;
        std::string m_name;

    private:
        friend class LayerStack;
        uint32_t m_id{};
    };

}

#endif //NEBULAENGINE_LAYER_H
