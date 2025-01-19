#pragma once

#include<GLCore.h>

#include "../GameObject2D.h"

struct Tile
{
    GameObject2D Obj;
    glm::vec2 TexCoords[4] = {};
};

struct Tileset
{
    int FirstId, LastId;
    int RowCount, ColumnCount;
    int TileCount;
    int TileWidth, TileHeight;
    std::string Name, Source;
    GLCore::Ref<GLCore::Texture2D> Texture;
};

using TilesetList = std::vector<GLCore::Ref<Tileset>>;
using TileMap = std::vector<std::vector<int> >;

class TileLayer
{
public:
    TileLayer(int tileWidth, int tileHeight, int rowCount, int columnCount,
        const TileMap& tileMap, const TilesetList& tilesets);

    void OnUpdate(GLCore::Timestep timestep);
    void OnRender();

    bool SolveCollision(GameObject2D& obj);

    const TileMap& GetTileMap() const { return m_TileMap; }

    void SetCollision(bool isEnabled) { m_isCollisionsEnabled = isEnabled; }
    bool GetCollision() const { return m_isCollisionsEnabled; }
private:
    void ComputeTileTexCoords();
private:
    int m_TileWidth, m_TileHeight;
    int m_RowCount, m_ColumnCount;
    TileMap m_TileMap;
    TilesetList m_Tilesets;
    std::unordered_map<int, GLCore::Scope<Tile>> m_TileData;
    bool m_isCollisionsEnabled;
};

