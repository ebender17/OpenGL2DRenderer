#pragma once

#include <GLCore.h>
#include "../General/GameObject2D.h"
#include "../Camera/Camera2DBaseController.h"

class LineBox2D
{
public:
    LineBox2D();
    ~LineBox2D();

    void DrawCollider(const GameObject2D& obj, const Camera2DBaseController& viewProjection);
private:
    glm::vec3 m_BoxColor = { 0.0f, 1.0f, 0.0f };
    GLCore::Ref<GLCore::Shader> m_VertexPosShader;
    uint32_t m_VBO;
    uint32_t m_VAO;
};

