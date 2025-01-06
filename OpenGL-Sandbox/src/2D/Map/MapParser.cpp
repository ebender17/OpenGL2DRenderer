#include "MapParser.h"

using namespace tinyxml2;

static MapParser* s_Instance = nullptr;

void MapParser::Init()
{
    GLCORE_ASSERT(s_Instance == nullptr, "Already initialized MapParser!");

    s_Instance = new MapParser();
}

void MapParser::Shutdown()
{
    delete s_Instance;
    s_Instance = nullptr;
}

bool MapParser::Load(const std::string& id, const std::string& source)
{
    return Parse(id, source);
}

void MapParser::Clean()
{
    for (auto entry : m_MapDict)
    {
        // TODO : delete pointer
        entry.second = nullptr;
    }
    m_MapDict.clear();
}

MapParser& MapParser::GetInstance()
{
    GLCORE_ASSERT(s_Instance != nullptr, "MapParser was not initialized!");
    return *s_Instance;
}

bool MapParser::Parse(const std::string& id, const std::string& source)
{
    XMLDocument xml;
    xml.LoadFile(source.c_str());

    if (xml.Error())
    {
        LOG_ERROR("TileMap XML file could not be loaded!");
        return false;
    }

    XMLElement* root = xml.RootElement();
    int rowCount = 0, columnCount = 0, tileWidth = 0, tileHeight = 0;

    columnCount = root->IntAttribute("width");
    rowCount = root->IntAttribute("height");
    tileWidth = root->IntAttribute("tilewidth");
    tileHeight = root->IntAttribute("tileheight");

    // Parse Tilesets
    TilesetList tilesets;
    tilesets.reserve(1); // TODO : get rid of magic number
    for (XMLElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        if (strcmp(element->Value(), "tileset") == 0)
            tilesets.emplace_back(ParseTileset(element));
    }

    // Parse TileLayers
    GameMap* gamemap = new GameMap(7); // TODO : get rid of magic number
    for (XMLElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        if (strcmp(element->Value(), "layer") == 0)
        {
            TileLayer* tileLayer = ParseTileLayer(element, tilesets, tileWidth, tileHeight, rowCount, columnCount);
            gamemap->GetMapLayers().emplace_back(tileLayer);
        }
    }

    m_MapDict[id] = gamemap;
    return true;
}

Tileset* MapParser::ParseTileset(XMLElement* xmlTileset)
{
    Tileset* tileset = new Tileset();
    tileset->Name = xmlTileset->Attribute("name");

    tileset->FirstId = xmlTileset->IntAttribute("firstgid");

    tileset->TileCount = xmlTileset->IntAttribute("tilecount");
    tileset->LastId = (tileset->FirstId - 1) + tileset->TileCount; // first id == 1, cannot just set == tilecount if more than one tileset in a map

    tileset->ColumnCount = xmlTileset->IntAttribute("columns");
    tileset->RowCount = tileset->TileCount / tileset->ColumnCount;
    tileset->TileWidth = xmlTileset->IntAttribute("tilewidth");

    XMLElement* image = xmlTileset->FirstChildElement();
    tileset->Source = image->Attribute("source");

    return tileset;
}

TileLayer* MapParser::ParseTileLayer(XMLElement* xmlLayer, TilesetList tilesets, int tileWidth, int tileHeight, int rowCount, int columnCount)
{
    XMLElement* data;
    for (XMLElement* element = xmlLayer->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        if (strcmp(element->Value(), "data") == 0)
        {
            data = element;
            break; // should only be 1 'data' element per layer
        }
    }

    // Parse TileMap
    std::string matrix(data->GetText());
    std::istringstream iss(matrix);
    std::string id;

    TileMap tilemap(rowCount, std::vector<int>(columnCount, 0)); // TODO : since this is stack allocated, will it cause issues when initializing the layer?

    for (int row = 0; row < rowCount; row++)
    {
        for (int col = 0; col < columnCount; col++)
        {
            getline(iss, id, ','); // all tile ids are seperate by a comma
            std::stringstream converter(id);
            converter >> tilemap[row][col];

            if (!iss.good()) // check for end of file
                break;
        }
    }
    return (new TileLayer(tileWidth, tileHeight, rowCount, columnCount, tilemap, tilesets));
}
