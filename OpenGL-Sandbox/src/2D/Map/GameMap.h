#pragma once

#include <GLCore.h> // TODO : do we need this include here?

#include "TileLayer.h"

class GameMap
{
public:
    GameMap(unsigned int reserveLayerCount);

    virtual void OnUpdate(GLCore::Timestep timestep);
    void OnRender();

    std::vector<GLCore::Ref<TileLayer>>& GetMapLayers() { return m_Layers; }
private:
    std::vector< GLCore::Ref<TileLayer>> m_Layers;
};