#include "World.h"


World::World() : mapData(ROWS, std::vector<int>(COLS, 0)) {
    tiles.load();  // Load tiles data
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

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            cout << mapData[i][j] << " ";
        }
        cout << endl;
    }
}

void World::drawMap(Window& canvas, int wy) {
    // Now mapData is accessible here
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            int id = mapData[row][col];
            int x = col * TILE_SIZE;
            int y = row * TILE_SIZE - wy; // Adjusting for wy if needed
            tiles[id].draw(canvas, x, y);
        }
    }
}
