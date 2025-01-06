#pragma once

#include <GLCore.h>

#include "TileLayer.h"

class GameMap
{
public:
    GameMap(unsigned int reserveLayerCount);

    virtual void OnUpdate(GLCore::Timestep timestep);
    void OnRender();

    std::vector<TileLayer*>& GetMapLayers() { return m_Layers; }
private:
    std::vector<TileLayer*> m_Layers; // TODO : convert to smart pointers
};