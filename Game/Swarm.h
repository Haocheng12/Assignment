#pragma once
#ifndef SWARM_H
#define SWARM_H
#include "GamesEngineeringBase.h"
#include"Object.h"

using namespace GamesEngineeringBase;
using namespace std;

class Swarm {
private:
    Enemy** enemies;// Dynamic array of Enemy pointers
    int maxEnemies;  // Max number of enemies in the swarm
    int numEnemies;  // Current number of enemies
    float timeElapsed = 0.0f;
    float spawnInterval = 2.0f; // Spawn a new enemy every 2 seconds

public:

    Swarm();
    ~Swarm();
    void update(Window& canvas, Player& player, float dt, float playerX, float playerY);
    void draw(Window& canvas);
    void addEnemy(Enemy* enemy);  // Method to add a new enemy
    void removeEnemy(int index);  // Method to remove an enemy by index
    int getNumEnemies() const;  // Getter for the current number of enemies
    Enemy** getEnemies() const;
};
#endif // OBJECT_H