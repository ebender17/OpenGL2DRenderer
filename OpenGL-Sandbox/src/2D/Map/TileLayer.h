#pragma once

#include "MapLayer.h"

struct Tileset {
    int FirstId, LastId;
    int RowCount, ColumnCount;
    int TileCount, TileSize;
    std::string Name, Source;
};

using TilesetList = std::vector<Tileset*>;
using TileMap = std::vector<std::vector<int> >;

class TileLayer : MapLayer
{
public:
    TileLayer(int tileSize, int rowCount, int columnCount, const TileMap& tileMap, const TilesetList& tilesets);

    virtual void OnUpdate(GLCore::Timestep timestep) override;
    virtual void OnRender() override;

    const TileMap& GetTileMap() const { return m_TileMap; }
private:
    int m_TileSize;
    int m_RowCount, m_ColumnCount;
    TileMap m_TileMap;
    TilesetList m_Tilesets;
};

