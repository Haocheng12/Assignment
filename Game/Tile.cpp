// Tile.cpp
#include "Tile.h"


vector<int> split(const string& line) {
    vector<int> result;
    stringstream ss(line);
    string token;
    while (getline(ss, token, ',')) {
        if (!token.empty()) {
            result.push_back(stoi(token));
        }
    }
    return result;
}

bool loadTileMap(const std::string& filename, std::vector<std::vector<int>>& tileMap, int& tileswide, int& tileshigh) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file.\n";
        return false;
    }

    std::string line;
    while (getline(file, line)) {
        if (line.find("tileswide") != std::string::npos) {
            tileswide = std::stoi(line.substr(line.find(" ")));
        }
        else if (line.find("tileshigh") != std::string::npos) {
            tileshigh = std::stoi(line.substr(line.find(" ")));
        }
        else if (line.find("layer") != std::string::npos) {
            // Now we parse tile IDs
            for (int row = 0; row < tileshigh; ++row) {
                getline(file, line);
                tileMap.push_back(split(line));
            }
        }
    }
    file.close();
    return true;
}


Tile::Tile() {
}

void Tile::load(string path) {
    image.load(path);  // Load the image from the provided path
}

void Tile::draw(Window& canvas, int x,int y) {
    // Draw the tile on the canvas at the specified Y coordinate
    for (unsigned int i = 0; i < image.height; i++) {
        // Bounds checking
        if (y + i >= 0 && (y + i) < (canvas.getHeight())) {
            for (unsigned int j = 0; j < image.width; j++) {
                if(x + j >= 0 && (x + j) < (canvas.getWidth()))
                canvas.draw(x+j, y + i, image.atUnchecked(j, i));
            }
        }
    }
}

void TileSet::load() {
    for (int i = 0; i < size; i++) {
        // Load each tile from the Resources folder with a filename based on index
        string filename = "Resources/tiles/" + to_string(i) + ".png";
        tiles[i].load(filename);
    }
}

Tile& TileSet::operator[](int index) {
    return tiles[index]; // Return the tile at the given index
}
