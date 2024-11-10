#include "World.h"


World::World() {
    // Dynamically allocate memory for the 2D array
    mapData = new int* [ROWS];
    for (int i = 0; i < ROWS; ++i) {
        mapData[i] = new int[COLS](); // Initialize all elements to 0
    }
    tiles.load(); // Load tiles data
    surviveMode = true;
}
World::~World() {
    for (int i = 0; i < ROWS; ++i) {
        delete[] mapData[i];
    }
    delete[] mapData;
}
int** World::getMapData() const {
    return mapData;
}
void World::load() {
    std::ifstream mapFile("Resources/tiles/map.txt");
    std::string line;
    int row = 0;

    // Read each line from the file
    while (getline(mapFile, line) && row < ROWS) {
        std::stringstream ss(line);
        std::string value;
        int col = 0;

        // Split the line by commas
        while (getline(ss, value, ',') && col < COLS) {
            mapData[row][col] = std::stoi(value); // Convert string to integer
            col++;
        }
        row++;
    }
    mapFile.close();
}




void World::drawMap(Window& canvas, int _x, int _y) {
    int tileX = _x / TILE_SIZE;
    int tileY = _y / TILE_SIZE;

    int offsetX = _x % TILE_SIZE;
    int offsetY = _y % TILE_SIZE;

    const int maxRows = 24;
    const int maxCols = 32;

    for (int row = 0; row <= maxRows; ++row) {
        if (tileY + row >= ROWS) break;
        for (int col = 0; col <= maxCols; ++col) {
            if (tileX + col >= COLS) break;

            int id = mapData[tileY + row][tileX + col];

            int drawX = col * TILE_SIZE - offsetX;
            int drawY = row * TILE_SIZE - offsetY;

            tiles[id].draw(canvas, drawX, drawY);
        }
    }
}

