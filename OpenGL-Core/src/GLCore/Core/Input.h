#pragma once

#include "GLCore/Core/KeyCodes.h"
#include "GLCore/Core/MouseCodes.h"

#include <glm/glm.hpp>

namespace GLCore {

    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMouseButtonPressed(MouseCode button);
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}