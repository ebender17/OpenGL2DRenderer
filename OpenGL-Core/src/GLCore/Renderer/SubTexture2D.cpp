#include "glpch.h"
#include "SubTexture2D.h"

namespace GLCore {

    SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& minTexCoord, const glm::vec2& maxTexCoord)
        : m_Texture(texture)
    {
        m_TexCoords[0] = { minTexCoord.x, minTexCoord.y };
        m_TexCoords[1] = { maxTexCoord.x, minTexCoord.y };
        m_TexCoords[2] = { maxTexCoord.x, maxTexCoord.y };
        m_TexCoords[3] = { minTexCoord.x, maxTexCoord.y };
    }

    Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
    {
        glm::vec2 minTexCoord = { (coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
        glm::vec2 maxTexCoord = { ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
        return CreateRef<SubTexture2D>(texture, minTexCoord, maxTexCoord);
    }

}

