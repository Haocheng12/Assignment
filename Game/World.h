#pragma once
// World.h
#ifndef WORLD_H
#define WORLD_H

#include "GamesEngineeringBase.h"
#include "Tile.h" // Include the header file for Tile instead of "Tile.cpp"
#include <string>

using namespace GamesEngineeringBase;
using namespace std;

const int maxSize = 24;

class World {
    TileSet tiles;
    int a[maxSize];

public:
    World();
    void draw(Window& canvas, int wy);
};

#endif // WORLD_H
