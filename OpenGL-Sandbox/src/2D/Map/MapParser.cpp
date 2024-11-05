#include "MapParser.h"

using namespace tinyxml2;

static MapParser* s_Instance = nullptr;

// TODO : make sure to call this!
void MapParser::Init()
{
    GLCORE_ASSERT(s_Instance != nullptr, "Already initialized MapParser!");

    s_Instance = new MapParser();
}

// TODO : make sure to call this!
void MapParser::Shutdown()
{
    delete s_Instance;
    s_Instance = nullptr;
}

bool MapParser::Load()
{
    // TODO : pass in level name & filepath
    return Parse("level1", "assets/textures/trainer - sapphire.png");
}

void MapParser::Clean()
{
}

MapParser& MapParser::GetInstance()
{
    GLCORE_ASSERT(s_Instance == nullptr, "MapParser was not initialized!");
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
    int rowCount, columnCount, tileSize;

    root->IntAttribute("width", columnCount);
    root->IntAttribute("height", rowCount);
    root->IntAttribute("tilewidth", tileSize);

    // Parse Tilesets
    TilesetList tilesets;
    tilesets.reserve(5); // TODO : get rid of magic number? how many tilesets do we normally use?
    for (XMLElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        if (element->Value() == "tileset")
            tilesets.emplace_back(ParseTileset(element));
    }

    // Parse TileLayers
    GameMap* gamemap = new GameMap(7); // TODO : get rid of magic number? how many layers do we normally have?
    for (XMLElement* element = root->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        if (element->Value() == "layer")
        {
            TileLayer* tileLayer = ParseTileLayer(element, tilesets, tileSize, rowCount, columnCount);
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
    xmlTileset->IntAttribute("firstgid", tileset->FirstId);

    xmlTileset->IntAttribute("tilecount", tileset->TileCount);
    tileset->LastId = (tileset->FirstId + tileset->TileCount);

    xmlTileset->IntAttribute("columns", tileset->ColumnCount);
    tileset->RowCount = tileset->TileCount / tileset->ColumnCount;
    xmlTileset->IntAttribute("tilewidth", tileset->TileSize);

    XMLElement* image = xmlTileset->FirstChildElement();
    tileset->Source = image->Attribute("source");

    // TODO : check these strings have no typos
    return tileset;
}

TileLayer* MapParser::ParseTileLayer(XMLElement* xmlLayer, TilesetList tilesets, int tileSize, int rowCount, int columnCount)
{
    XMLElement* data;
    for (XMLElement* element = xmlLayer->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        if (element->Value() == "data")
        {
            data = element;
            break; // should only be 1 'data' element per layer
        }
    }

    // Parse TileMap
    std::string matrix(data->GetText());
    std::istringstream iss(matrix);
    std::string id;

    TileMap tilemap(rowCount, std::vector<int>(columnCount, 0));

    for (int row = 0; row < rowCount; row++)
    {
        for (int col = 0; col < columnCount; col++)
        {
            getline(iss, id, ','); // all ids are seperate by a comma
            std::stringstream converter(id);
            converter >> tilemap[row][col];

            if (!iss.good()) // check for end of file
                break;
        }
    }
    return (new TileLayer(tileSize, rowCount, columnCount, tilemap, tilesets));
}
