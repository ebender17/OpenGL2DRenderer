#pragma once

#include "GLCore/Core/Core.h"
#include "GLCore/Core/Timestep.h"
#include "GLCore/Events/Event.h"

namespace GLCore {

    class Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep timestep) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

    protected:
        std::string m_DebugName;
    };

}