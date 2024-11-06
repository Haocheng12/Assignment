#pragma once
// Tile.h

#ifndef TILE_H
#define TILE_H

#include "GamesEngineeringBase.h"
#include <string>
#include<vector>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace GamesEngineeringBase;
using namespace std;

const unsigned int tileNum = 24; // Number of tiles
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;

class Tile {
    Image image; // The image for the tile

public:
    Tile();                  // Constructor declaration
    void load(string path);  // Load the tile from the specified path
    void draw(Window& canvas, int x,int y);  // Draw the tile on the canvas
};

class TileSet {
public:
    Tile tiles[tileNum];      // Array of tiles
    unsigned int size = tileNum; // Size of the tile set

    TileSet();                // Constructor declaration
    void load();              // Load all tiles
    Tile& operator[](int index); // Access a specific tile by index
};

#endif // TILE_H
