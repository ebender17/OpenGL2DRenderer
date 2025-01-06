#pragma once

#include<GLCore.h>

struct Tileset {
    int FirstId, LastId;
    int RowCount, ColumnCount;
    int TileCount;
    int TileWidth, TileHeight;
    std::string Name, Source;
    GLCore::Ref<GLCore::Texture2D> Texture;
};

using TilesetList = std::vector<Tileset*>; // TODO : convert to smart pointers
using TileMap = std::vector<std::vector<int> >;

class TileLayer
{
public:
    TileLayer(int tileWidth, int tileHeight, int rowCount, int columnCount, const TileMap& tileMap, const TilesetList& tilesets);

    void OnUpdate(GLCore::Timestep timestep);
    void OnRender();

    const TileMap GetTileMap() const { return m_TileMap; } // TODO : do not return as copy
private:
    int m_TileWidth, m_TileHeight;
    int m_RowCount, m_ColumnCount;
    TileMap m_TileMap;
    TilesetList m_Tilesets;
};

