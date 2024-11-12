#pragma once
// World.h
#ifndef WORLD_H
#define WORLD_H

#include "GamesEngineeringBase.h"
#include "Tile.h" // Include the header file for Tile instead of "Tile.cpp"
#include <string>
#include "Object.h"
using namespace GamesEngineeringBase;
using namespace std;

const int TILE_SIZE = 32;
const int MAX_ROWS = 74;    // For the fixed map size
const int MAX_COLS = 75;
static const int ENDLESS_SIZE = 35;
class World {
    TileSet tiles;
    int** fixedMap;         // For surviveMode (fixed map)
    int** endlessMap;       // For endlessMode (dynamic map)
    int centerX;
    int centerY;
   

public:
    bool surviveMode;
    World();
    ~World();
    void load(bool loaded);
    int** getMap();
   
    int getTileAt(int globalTileX, int globalTileY);
    void updateMap(Player& player,int playerX, int playerY);
    void drawMap(Window& canvas, Player& player, int playerX, int playerY);
    void saveMap()const;
    void loadFixedMap(); // New function for loading fixed map
    void loadEndlessMap();
};

#endif // WORLD_H
