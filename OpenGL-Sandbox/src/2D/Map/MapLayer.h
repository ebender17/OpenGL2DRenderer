#pragma once

#include <GLCore.h>

class MapLayer
{
public:
    MapLayer() = default;

    virtual void OnUpdate(GLCore::Timestep timestep) = 0;
    virtual void OnRender() = 0;
};

