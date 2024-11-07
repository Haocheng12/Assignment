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
    vector<vector<int>> mapData;

public:
    World();
    void load();
    void drawMap(Window& canvas, int wy);
};

#endif // WORLD_H
