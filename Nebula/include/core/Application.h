//
// Created by michal-swiatek on 31.10.2023.
// Github: https://github.com/michal-swiatek
//

#ifndef NEBULAENGINE_APPLICATION_H
#define NEBULAENGINE_APPLICATION_H

#include <string>

#include "Core.h"
#include "LayerStack.h"

#include "events/Event.h"
#include "events/EventManager.h"

namespace nebula {

    class NEBULA_API Application
    {
    public:
        explicit Application(std::string ph_1, const std::string& logger_name = "APP");
        virtual ~Application() = default;

        void onEvent(Event& event);

        void run();

        template <typename T, typename... Args>
        LayerStack::LayerID pushLayer(Args&&... args)
        {
            auto layer = new T(std::forward<Args>(args)...);    //  TODO: Replace with memory allocator
            layer->onAttach();
            return m_layer_stack.pushLayer(layer);
        }

        template <typename T, typename... Args>
        LayerStack::LayerID pushOverlay(Args&&... args)
        {
            auto layer = new T(std::forward<Args>(args)...);    //  TODO: Replace with memory allocator
            layer->onAttach();
            return m_layer_stack.pushOverlay(layer);
        }

        Scope<Layer> popLayer(LayerStack::LayerID layer_id) { return m_layer_stack.popLayer(layer_id); }
        Scope<Layer> popOverlay(LayerStack::LayerID layer_id) { return m_layer_stack.popOverlay(layer_id); }

    private:
        std::string m_name;

        LayerStack m_layer_stack;
        EventManager m_event_manager;
    };

    Application* createApplication(int argc, char** argv);

}

#endif //NEBULAENGINE_APPLICATION_H
