#include "World.h"


World::World() : fixedMap(nullptr), endlessMap(nullptr), surviveMode(true) {
    
    tiles.load();
    
    
}



World::~World() {
    // Deallocate fixed map
    if (fixedMap) {
        for (int i = 0; i < MAX_ROWS; ++i) {
            delete[] fixedMap[i];
        }
        delete[] fixedMap;
    }

    // Deallocate endless map
    if (endlessMap) {
        for (int i = 0; i < ENDLESS_SIZE; ++i) {
            delete[] endlessMap[i];
        }
        delete[] endlessMap;
    }
}

int** World::getMap() {
    if (surviveMode) {
        return fixedMap;
    }
    return endlessMap;
    
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

void World::loadEndlessMap() {
    std::ifstream mapFile("EndlessMap.txt");
    std::string line;
    int row = 0;

    while (getline(mapFile, line) && row < 35) {
        std::stringstream ss(line);
        std::string value;
        int col = 0;

        while (getline(ss, value, ',') && col < 35) {
            fixedMap[row][col] = std::stoi(value);
            col++;
        }
        row++;
    }
    mapFile.close();
}

void World::load(bool loaded) {

    srand(static_cast<unsigned int>(time(0))); // Seed random generator

    // Allocate memory based on mode
    if (surviveMode) {
        
        // Allocate memory for the fixed map (survive mode)
        fixedMap = new int* [MAX_ROWS];
        for (int i = 0; i < MAX_ROWS; ++i) {
            fixedMap[i] = new int[MAX_COLS]();
        }
        loadFixedMap();
    }
    else {
       
            endlessMap = new int* [ENDLESS_SIZE];
            for (int i = 0; i < ENDLESS_SIZE; ++i) {
                endlessMap[i] = new int[ENDLESS_SIZE];
                for (int j = 0; j < ENDLESS_SIZE; ++j) {
                    endlessMap[i][j] = rand() % 15; // Fill with random tile IDs
                }
            }
            centerX = ENDLESS_SIZE * 32 / 2;
            centerY = ENDLESS_SIZE * 32 / 2;
            //loadEndlessMap();
        }
        
       
    
}

int World::getTileAt(int globalTileX, int globalTileY) {
    if (surviveMode) {
        // Bounds check for fixed map
        return fixedMap[globalTileY][globalTileX];
    }
    else {
      
        return endlessMap[globalTileY][globalTileX];
    }
}

void World::updateMap(Player& player, int playerX, int playerY) {
    const int threshold =  TILE_SIZE; // Distance from center before shifting
    cout << "playerX " << player.x << "playerY " << player.y << endl;
    cout << "threshold " << threshold  << endl;
    cout << "centerX " << centerX << "centerY " << centerY << endl;
    if (playerX < centerX - threshold) {
        // Shift tiles right and generate new tiles on the left
        for (int i = 0; i < ENDLESS_SIZE; ++i) {
            for (int j = ENDLESS_SIZE - 1; j > 0; --j) {
                endlessMap[i][j] = endlessMap[i][j - 1];
            }
            endlessMap[i][0] = rand() % 15;
        }
        player.x+=32.0f;
    }
    else if (playerX > centerX + threshold) {
        // Shift tiles left and generate new tiles on the right
        for (int i = 0; i < ENDLESS_SIZE; ++i) {
            for (int j = 0; j < ENDLESS_SIZE - 1; ++j) {
                endlessMap[i][j] = endlessMap[i][j + 1];
            }
            endlessMap[i][ENDLESS_SIZE - 1] = rand() % 15;
        }
        player.x -= 32.0f;
    }

    if (playerY < centerY - threshold) {
        // Shift tiles down and generate new tiles on top
        for (int j = 0; j < ENDLESS_SIZE; ++j) {
            for (int i = ENDLESS_SIZE - 1; i > 0; --i) {
                endlessMap[i][j] = endlessMap[i - 1][j];
            }
            endlessMap[0][j] = rand() % 15;
        }
        player.y += 32.0f;
    }
    else if (playerY > centerY + threshold) {
        // Shift tiles up and generate new tiles at the bottom
        for (int j = 0; j < ENDLESS_SIZE; ++j) {
            for (int i = 0; i < ENDLESS_SIZE - 1; ++i) {
                endlessMap[i][j] = endlessMap[i + 1][j];
            }
            endlessMap[ENDLESS_SIZE - 1][j] = rand() % 15;
        }
        player.y -= 32.0f;
    }
}


void World::drawMap(Window& canvas, Player& player, int playerX, int playerY) {
    
    

    const int maxRows = 24; 
    const int maxCols = 32;

    int screenWidth = canvas.getWidth();
    int screenHeight = canvas.getHeight();

    float cameraX = playerX - screenWidth / 2;
    float cameraY = playerY - screenHeight / 2;

    int startTileX = static_cast<int>(cameraX) / TILE_SIZE;
    int startTileY = static_cast<int>(cameraY) / TILE_SIZE;
    
    int offsetX = static_cast<int>(cameraX) % TILE_SIZE;
    int offsetY = static_cast<int>(cameraY) % TILE_SIZE;
    
    int numRows = screenHeight / TILE_SIZE + 1;
    int numCols = screenWidth / TILE_SIZE + 1;
  
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            int globalTileX = startTileX + col;
            int globalTileY = startTileY + row;

            int tileID = getTileAt(globalTileX, globalTileY);

            // Calculate the position 
            int drawX = col * TILE_SIZE - offsetX;
            int drawY = row * TILE_SIZE - offsetY;

            tiles[tileID].draw(canvas, drawX, drawY);
        }
    }
    if (!surviveMode) {
        updateMap(player, playerX, playerY);
    }
}


void World::saveMap()const {
    std::ofstream outFile("EndlessMap.txt");

    if (outFile.is_open()) {
        // Iterate through each row and column to store the 2D map
        for (int i = 0; i < 35; ++i) {
            for (int j = 0; j < 35; ++j) {
                outFile << endlessMap[i][j];  // Write the integer value
                if (j < 34) {
                    outFile << ",";  // Add a comma between columns
                }
            }
            outFile << "\n";  // Newline after each row
        }
        std::cout << "Map saved successfully.\n";
    }
    else {
        std::cerr << "Failed to open the file for writing.\n";
    }

    outFile.close();
}
