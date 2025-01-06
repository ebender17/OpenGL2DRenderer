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

    static GLCore::Ref<GameMap> Load(const std::string& source);

    static MapParser& GetInstance();
private:
    MapParser() = default;
    ~MapParser() = default;

    static GLCore::Ref<GameMap> Parse(const std::string& source);
    static GLCore::Ref<Tileset> ParseTileset(tinyxml2::XMLElement* xmlTileset);
    static GLCore::Ref<TileLayer> ParseTileLayer(tinyxml2::XMLElement* xmlLayer, TilesetList tilesets, int tileWidth,
        int tileHeight, int rowCount, int columnCount);
};

