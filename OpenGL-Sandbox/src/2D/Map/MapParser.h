#pragma once

#include "GameMap.h"
#include "TileLayer.h"

#include <tinyxml/tinyxml2.h>

class MapParser
{
public:
    MapParser(const MapParser&) = delete;
    MapParser& operator=(const MapParser&) = delete;

    static void Init();
    static void Shutdown();

    bool Load(const std::string& id, const std::string& source);
    void Clean(); // TODO : remove

    static MapParser& GetInstance();
    GameMap* GetMap(const std::string& id) { return m_MapDict[id]; }
private:
    MapParser() = default;
    ~MapParser() = default;

    bool Parse(const std::string& id, const std::string& source);
    Tileset* ParseTileset(tinyxml2::XMLElement* xmlTileset); // TODO : use smart pointers
    TileLayer* ParseTileLayer(tinyxml2::XMLElement* xmlLayer, TilesetList tilesets, int tileWidth,
        int tileHeight, int rowCount, int columnCount); // TODO : use smart pointers

private:
    std::map<std::string, GameMap*> m_MapDict; // TODO : move this data to GameMap or another class
    // Also make sure to free memory - in Clean() here
};

