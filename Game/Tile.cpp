// Tile.cpp
#include "Tile.h"

Tile::Tile() {
    // Constructor definition (empty if no special initialization needed)
}

void Tile::load(string path) {
    image.load(path);  // Load the image from the provided path
}

void Tile::draw(Window& canvas, int y) {
    // Draw the tile on the canvas at the specified Y coordinate
    for (unsigned int i = 0; i < image.height; i++) {
        // Bounds checking
        if (y + i >= 0 && (y + i) < (canvas.getHeight())) {
            for (unsigned int n = 0; n < image.width; n++) {
                canvas.draw(n, y + i, image.atUnchecked(n, i));
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
