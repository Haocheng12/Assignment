#pragma once
// World.h
#ifndef WORLD_H
#define WORLD_H

#include "GamesEngineeringBase.h"
#include "Tile.h" // Include the header file for Tile instead of "Tile.cpp"
#include <string>

using namespace GamesEngineeringBase;
using namespace std;

const int TILE_NUM = 24;
const int ROWS = 42; // Set to the correct number of rows in map.txt
const int COLS = 42; // Set to the correct number of columns in map.txt
const int TILE_SIZE = 32;


class World {
    TileSet tiles;
    int** mapData;  
    
public:
    World();
    ~World(); // Destructor to free allocated memory
    void load();
    void drawMap(Window& canvas, int x,int y);
    int** getMapData() const;
};

#endif // WORLD_H
