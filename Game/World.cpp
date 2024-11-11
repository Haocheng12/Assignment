#include "World.h"


World::World() : fixedMap(nullptr) {
    srand(static_cast<unsigned int>(time(0))); // Seed random generator

    // Allocate memory for the fixed map (survive mode)
    fixedMap = new int* [MAX_ROWS];
    for (int i = 0; i < MAX_ROWS; ++i) {
        fixedMap[i] = new int[MAX_COLS]();
    }

    // Initial grid size (3x3 chunks) for endless mode
    chunkGridSize = 3;
    centerChunkX = chunkGridSize / 2;
    centerChunkY = chunkGridSize / 2;

    // Allocate memory for chunks
    chunks = new int** [chunkGridSize];
    for (int i = 0; i < chunkGridSize; ++i) {
        chunks[i] = new int* [chunkGridSize];
        for (int j = 0; j < chunkGridSize; ++j) {
            chunks[i][j] = nullptr; // Chunks start as uninitialized
        }
    }

    tiles.load();
    surviveMode = true; // Default mode
}
World::~World() {
    // Deallocate fixed map
    if (fixedMap) {
        for (int i = 0; i < MAX_ROWS; ++i) {
            delete[] fixedMap[i];
        }
        delete[] fixedMap;
    }

    // Deallocate chunks
    for (int i = 0; i < chunkGridSize; ++i) {
        for (int j = 0; j < chunkGridSize; ++j) {
            if (chunks[i][j] != nullptr) {
                delete[] chunks[i][j];
            }
        }
        delete[] chunks[i];
    }
    delete[] chunks;
}

int** World::getFixedMap()  {
    return fixedMap;         // For surviveMode (fixed map)
    ;
}

void World::loadFixedMap() {
    std::ifstream mapFile("Resources/tiles/map.txt");
    std::string line;
    int row = 0;

    while (getline(mapFile, line) && row < MAX_ROWS) {
        std::stringstream ss(line);
        std::string value;
        int col = 0;

        while (getline(ss, value, ',') && col < MAX_COLS) {
            fixedMap[row][col] = std::stoi(value);
            col++;
        }
        row++;
    }
    mapFile.close();
}
void World::load() {
    if (surviveMode) {
        loadFixedMap(); // Load the static map from file
    }
    else {
        // Generate random chunks for endless mode
        for (int i = 0; i < chunkGridSize; ++i) {
            for (int j = 0; j < chunkGridSize; ++j) {
                generateChunk(i, j);
            }
        }
    }
}


int World::getTileAt(int globalTileX, int globalTileY) {
    if (surviveMode) {
        // Bounds check for fixed map
        if (globalTileX < 0 || globalTileX >= MAX_COLS || globalTileY < 0 || globalTileY >= MAX_ROWS) {
            return 0; // Default tile (e.g., empty)
        }
        return fixedMap[globalTileY][globalTileX];
    }
    else {
        // For infinite map mode
        int chunkX = globalTileX / CHUNK_SIZE;
        int chunkY = globalTileY / CHUNK_SIZE;

        int tileX = globalTileX % CHUNK_SIZE;
        int tileY = globalTileY % CHUNK_SIZE;

        if (tileX < 0) {
            tileX += CHUNK_SIZE;
            chunkX--;
        }
        if (tileY < 0) {
            tileY += CHUNK_SIZE;
            chunkY--;
        }   

        generateChunk(chunkX, chunkY);
        return chunks[chunkX][chunkY][tileY * CHUNK_SIZE + tileX];
    }
}
void World::generateChunk(int chunkX, int chunkY) {
    // Ensure chunk indices are non-negative
    if (chunkX < 0 || chunkY < 0) return;

    // Expand chunk grid if needed
    if (chunkX >= chunkGridSize || chunkY >= chunkGridSize) {
        expandChunks(); // Expand the chunks array if out of bounds
    }

    // If the chunk is already generated, skip it
    if (chunks[chunkX][chunkY] != nullptr) return;

    // Allocate memory for the new chunk (32x32 tiles)
    chunks[chunkX][chunkY] = new int[CHUNK_SIZE * CHUNK_SIZE];

    // Seed the random number generator (optional: can use chunk coordinates for unique seed)
    srand(static_cast<unsigned int>(time(0)) + chunkX * 1000 + chunkY);

    // Fill the chunk with random tile IDs
    for (int i = 0; i < CHUNK_SIZE; ++i) {
        for (int j = 0; j < CHUNK_SIZE; ++j) {
            int maxTileID = 15; // Adjust based on the number of tile types you have
            chunks[chunkX][chunkY][i * CHUNK_SIZE + j] = rand() % maxTileID;
        }
    }
}

void World::expandChunks() {
    int newSize = chunkGridSize * 2; // Double the current grid size

    // Create a new chunks array with expanded size
    int*** newChunks = new int** [newSize];
    for (int i = 0; i < newSize; ++i) {
        newChunks[i] = new int* [newSize];
        for (int j = 0; j < newSize; ++j) {
            newChunks[i][j] = nullptr; // Initialize all to nullptr
        }
    }

    // Copy existing chunks to the new array, centered
    int offset = (newSize - chunkGridSize) / 2;
    for (int i = 0; i < chunkGridSize; ++i) {
        for (int j = 0; j < chunkGridSize; ++j) {
            newChunks[i + offset][j + offset] = chunks[i][j];
        }
    }

    // Delete the old chunks array
    for (int i = 0; i < chunkGridSize; ++i) {
        delete[] chunks[i];
    }
    delete[] chunks;

    // Update pointers and size
    chunks = newChunks;
    chunkGridSize = newSize;
}

void World::expandChunksIfNeeded(int playerX, int playerY) {
    // Determine the chunk coordinates where the player is located
    int playerChunkX = playerX / (CHUNK_SIZE * TILE_SIZE);
    int playerChunkY = playerY / (CHUNK_SIZE * TILE_SIZE);

    // Check if the player is near the edges of the currently loaded chunks
    // and expand if necessary
    if (playerChunkX < centerChunkX - chunkGridSize / 2 ||
        playerChunkX > centerChunkX + chunkGridSize / 2 ||
        playerChunkY < centerChunkY - chunkGridSize / 2 ||
        playerChunkY > centerChunkY + chunkGridSize / 2) {

        expandChunks();
    }
}

void World::drawMap(Window& canvas, int playerX, int playerY) {
    // Dynamically expand the chunk grid if needed before drawing
    expandChunksIfNeeded(playerX, playerY);

    // Define the size of the map in terms of rows and columns
    const int maxRows = 24; // Map height in tiles
    const int maxCols = 32; // Map width in tiles

    // Define screen dimensions
    int screenWidth = canvas.getWidth();
    int screenHeight = canvas.getHeight();

    // Center camera around player
    float cameraX = playerX - screenWidth / 2;
    float cameraY = playerY - screenHeight / 2;

    // Calculate the starting tile indices based on the camera position
    int startTileX = static_cast<int>(cameraX) / TILE_SIZE;
    int startTileY = static_cast<int>(cameraY) / TILE_SIZE;

    // Calculate the offset within the tile for smooth scrolling
    int offsetX = static_cast<int>(cameraX) % TILE_SIZE;
    int offsetY = static_cast<int>(cameraY) % TILE_SIZE;

    // Determine the number of tiles to draw based on the screen size
    int numRowsToDraw = screenHeight / TILE_SIZE + 1;
    int numColsToDraw = screenWidth / TILE_SIZE + 1;

    // Draw only the tiles that are visible on the screen
    for (int row = 0; row < numRowsToDraw; ++row) {
        for (int col = 0; col < numColsToDraw; ++col) {
            int globalTileX = startTileX + col;
            int globalTileY = startTileY + row;

            // Ensure we're not trying to draw tiles outside the map boundaries
            int tileID = getTileAt(globalTileX, globalTileY);

            // Calculate the position to draw the tile on the screen
            int drawX = col * TILE_SIZE - offsetX;
            int drawY = row * TILE_SIZE - offsetY;

            // Draw the tile at the calculated position
            tiles[tileID].draw(canvas, drawX, drawY);
        }
    }
}



