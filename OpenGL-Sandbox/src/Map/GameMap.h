#pragma once

#include "../General/GameObject2D.h"
#include "TileLayer.h"

class GameMap
{
public:
    GameMap(int rowCount, int columnCount, unsigned int reserveLayerCount);
    ~GameMap();

    virtual void OnUpdate(GLCore::Timestep timestep);
    void OnRender();
    void OnRenderWaterLayer();

    bool CheckCollision(const glm::vec2& objPosition, float width, float height) const;

    std::vector<GLCore::Ref<TileLayer>>& GetMapLayers() { return m_Layers; }
private:
    std::vector< GLCore::Ref<TileLayer>> m_Layers;
    int m_RowCount, m_ColumnCount;

    const float c_WaterZPosition = -0.07f;
    const float c_GroundZPosition = -0.05f;
};