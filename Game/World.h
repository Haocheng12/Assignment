#pragma once
// World.h
#ifndef WORLD_H
#define WORLD_H

#include "GamesEngineeringBase.h"
#include "Tile.h" // Include the header file for Tile instead of "Tile.cpp"
#include <string>

using namespace GamesEngineeringBase;
using namespace std;

const int TILE_SIZE = 32;
const int CHUNK_SIZE = 32;  // Each chunk is 32x32 tiles
const int MAX_ROWS = 74;    // For the fixed map size
const int MAX_COLS = 75;

class World {
    TileSet tiles;
    int** fixedMap;         // For surviveMode (fixed map)
    int*** chunks;          // For endless mode (dynamic map)
    int chunkGridSize;      // Size of the chunk grid
    int centerChunkX, centerChunkY; // Player's current chunk position
    

public:
    bool surviveMode;
    World();
    ~World();
    void load();
    int** getFixedMap();
    void generateChunk(int chunkX, int chunkY);
    void drawMap(Window& canvas, int playerX, int playerY);
    int getTileAt(int globalTileX, int globalTileY);
    void expandChunks();
    void loadFixedMap(); // New function for loading fixed map
    void expandChunksIfNeeded(int playerX, int playerY);
};

#endif // WORLD_H
