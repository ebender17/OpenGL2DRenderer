#pragma once

#include "GLCore/Core/Core.h"
#include "GLCore/Events/Event.h"

namespace GLCore {

    class GLCORE_API Layer
    {
    public:
        Layer(const std::string& name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

    protected:
        std::string m_DebugName;
    };

}