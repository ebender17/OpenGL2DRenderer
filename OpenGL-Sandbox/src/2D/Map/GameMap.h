#pragma once

#include "../General/GameObject2D.h"
#include "TileLayer.h"

class GameMap
{
public:
    GameMap(unsigned int reserveLayerCount);

    virtual void OnUpdate(GLCore::Timestep timestep);
    void OnRender();

    void SolveCollision(GameObject2D& obj) const;

    std::vector<GLCore::Ref<TileLayer>>& GetMapLayers() { return m_Layers; }
private:
    std::vector< GLCore::Ref<TileLayer>> m_Layers;
};