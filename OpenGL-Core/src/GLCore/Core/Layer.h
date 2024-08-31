#pragma once

#include "Core.h"
#include "Timestep.h"
#include "GLCore/Events/Event.h"

namespace GLCore {

    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep timestep) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

    protected:
        std::string m_DebugName;
    };

}