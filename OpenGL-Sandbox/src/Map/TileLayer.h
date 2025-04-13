#pragma once

#include<GLCore.h>

#include "../General/GameObject2D.h"

struct TileData
{
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
    enum class Type
    {
        Unknown = 0,
        Water = 1,
    };
public:
    TileLayer(int tileWidth, int tileHeight, int rowCount, int columnCount,
        const TileMap& tileMap, const TilesetList& tilesets, const std::string& type);

    void OnUpdate(GLCore::Timestep timestep);
    void OnRender(float zPosition = -0.05f);

    bool CheckCollision(const glm::vec2& objPosition, float width, float height);

    TileLayer::Type GetTileLayerType() const { return m_TileLayerType; }
    const TileMap& GetTileMap() const { return m_TileMap; }
    bool GetIsCollisionEnabled() const { return m_isCollisionsEnabled; }

    void SetIsCollisionEnabled(bool isEnabled) { m_isCollisionsEnabled = isEnabled; }
private:
    void ComputeTileTexCoords();
    TileLayer::Type StringToType(const std::string& typeStr); // TODO : make static? and public?
    bool IntersectsTile(const glm::vec2& tilePosition, const glm::vec2& objPosition) const;
private:
    int m_TileWidth, m_TileHeight;
    int m_RowCount, m_ColumnCount;
    TileMap m_TileMap;
    TilesetList m_Tilesets; // TODO : only support one tileset for now

    bool m_isCollisionsEnabled;
    TileLayer::Type m_TileLayerType;

    std::unordered_map<int, TileData> m_TileData; // TODO : make a scoped pointer

    const glm::vec2 c_SpriteSize = { 1.0f, 1.0f };
};

