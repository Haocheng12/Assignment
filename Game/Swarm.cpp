#include"Swarm.h"


Swarm::Swarm() {
    maxEnemies = 100;  // Maximum number of enemies allowed
    numEnemies = 0;    // Initially no enemies
    enemies = new Enemy * [maxEnemies];

}

Swarm::~Swarm() {
    // Delete all dynamically allocated enemies to avoid memory leaks
    for (int i = 0; i < numEnemies; ++i) {
        delete enemies[i];
    }
    delete[] enemies;  // Free the dynamic array
}

void Swarm::addEnemy(Enemy* enemy) {
    // Check if there is space for a new enemy
    if (numEnemies < maxEnemies) {
        enemies[numEnemies] = enemy;  // Add the new enemy to the array
        ++numEnemies;  // Increment the number of enemies
    }
    else {
        std::cerr << "Error: Cannot add more enemies, max limit reached!" << std::endl;
    }
}

void Swarm::removeEnemy(int index) {
    if (index >= 0 && index < numEnemies) {
        delete enemies[index];  // Delete the enemy at the given index
        // Shift remaining enemies to the left
        for (int i = index; i < numEnemies - 1; ++i) {
            enemies[i] = enemies[i + 1];
        }
        --numEnemies;  // Decrease the number of enemies
    }
}

int Swarm::getNumEnemies() const {
    return numEnemies;  // Return the current number of enemies
}
Enemy** Swarm::getEnemies() const {
    return enemies;  // Return the current number of enemies
}

void Swarm::update(Window& canvas, Player& player, float dt, float playerX, float playerY) {
    timeElapsed += dt;

    // Spawn a new enemy if enough time has passed
    if (timeElapsed > spawnInterval) {
        // Determine random spawn location outside the screen
        int spawnX, spawnY;

        // Randomly choose one of four sides of the screen for spawning
        int side = rand() % 4;
        int enemyType = rand() % 3;
        switch (side) {
        case 0: // Spawn above the screen
            spawnX = rand() % (canvas.getWidth() - 64); // Ensure it spawns fully visible horizontally
            spawnY = -64; // Spawn just above the screen
            break;
        case 1: // Spawn below the screen
            spawnX = rand() % (canvas.getWidth() - 64);
            spawnY = canvas.getHeight() + 64; // Spawn just below the screen
            break;
        case 2: // Spawn to the left of the screen
            spawnX = -64;
            spawnY = rand() % (canvas.getHeight() - 64); // Ensure it spawns fully visible vertically
            break;
        case 3: // Spawn to the right of the screen
            spawnX = canvas.getWidth() + 64;
            spawnY = rand() % (canvas.getHeight() - 64);
            break;
        }

        Enemy* newEnemy = nullptr;
        switch (enemyType) {
        case 0:
            newEnemy = new BatEnemy(spawnX, spawnY);
            break;
        case 1:
            newEnemy = new DogEnemy(spawnX, spawnY);
            break;
        case 2:
            newEnemy = new scorpionEnemy(spawnX, spawnY);
            break;
        }

        if (newEnemy != nullptr) {
            addEnemy(newEnemy);  // Add the newly created enemy
        }

        timeElapsed = 0;
    }

    // Update each enemy and check for collisions
    for (int i = 0; i < numEnemies; ) {
        Enemy* currentEnemy = enemies[i];

        if (player.screenX > 511.0 && player.screenX < 513.0) {
            currentEnemy->x -= playerX;
        }
        if (player.screenY > 383.0 && player.screenY < 385.0) {
            currentEnemy->y -= playerY;
        }

        currentEnemy->updateEnemy(canvas, player, dt);

        if (currentEnemy->checkCollision(player)) {
            std::cout << "ENEMY COLLIDED WITH PLAYER" << std::endl;
            removeEnemy(i);  // Remove enemy from array
        }
        else {
            ++i;
        }
    }
}

void Swarm::draw(Window& canvas) {
    for (int i = 0; i < numEnemies; ++i) {
        enemies[i]->draw(canvas);  // Draw each enemy in the array
    }
}