#pragma once

#include <GLCore.h>

#include "MapLayer.h"

class GameMap
{
public:
    GameMap(unsigned int reserveLayerCount);

    virtual void OnUpdate(GLCore::Timestep timestep);
    void OnRender();

    std::vector<MapLayer*>& GetMapLayers() { return m_MapLayers; }
private:
    std::vector<MapLayer*> m_MapLayers; // TODO : convert to smart pointers
};