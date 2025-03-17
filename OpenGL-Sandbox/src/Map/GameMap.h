#pragma once

#include "../General/GameObject2D.h"
#include "TileLayer.h"

class GameMap
{
public:
    GameMap(int rowCount, int columnCount, unsigned int reserveLayerCount);
    ~GameMap();

    void GenerateWaterMaskTexture();

    virtual void OnUpdate(GLCore::Timestep timestep);
    void OnRender();

    bool CheckCollision(const glm::vec2& objPosition, float width, float height) const;

    std::vector<GLCore::Ref<TileLayer>>& GetMapLayers() { return m_Layers; }
    unsigned int GetTilemapTexture() { return m_WaterMaskTexture; }
private:
    std::vector< GLCore::Ref<TileLayer>> m_Layers;
    int m_RowCount, m_ColumnCount;

    uint32_t m_WaterMaskTexture;
};