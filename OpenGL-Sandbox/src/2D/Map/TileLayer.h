#pragma once

#include<GLCore.h>

#include "../General/GameObject2D.h"

struct TileData
{
    glm::vec2 Position = {};
    glm::vec2 TexCoords[4] = {};
};

struct Tileset
{
    unsigned int FirstId, LastId;
    unsigned int RowCount, ColumnCount;
    unsigned int TileCount;
    unsigned int TileWidth, TileHeight;
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

    void SetIsCollisionEnabled(bool isEnabled) { m_isCollisionsEnabled = isEnabled; }
    bool GetIsCollisionEnabled() const { return m_isCollisionsEnabled; }
private:
    void ComputeTileTexCoords();
private:
    int m_TileWidth, m_TileHeight;
    int m_RowCount, m_ColumnCount;
    TileMap m_TileMap;
    TilesetList m_Tilesets; // TODO : only support one tileset for now

    bool m_isCollisionsEnabled;

    std::unordered_map<int, TileData> m_TileData; // TODO : make a scoped pointer

    const glm::vec2 c_SpriteSize = { 1.0f, 1.0f };
};

