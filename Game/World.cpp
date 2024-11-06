#include "World.h"


World::World() {
    tiles.load();
    for (int i = 0; i < maxSize; i++) {
        a[i] = i; // Assuming tileNum is defined in Tile.h or elsewhere
    }
}

void World::draw(Window& canvas, int wy) {
   
    tiles[0].draw(canvas, 0 , 0);
    
}
