#include "Object.h"


Object::Object(){}

Object::Object(float _x, float _y, int _speed, const string& path) {
    x = _x;
    y = _y;
    speed = _speed;
    if (!image.load(path)) {
        std::cerr << "Failed to load image: " << path << std::endl;
        exit(1); // Handle the error appropriately
    }
}
void Object::update(float _x, float _y) {
    x += _x;
    y += _y;
}

void Object::update(Window& canvas, float _x, float _y) {
    x += _x;
    y += _y;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + image.width >= canvas.getWidth()) x = canvas.getWidth() - image.width;
    if (y + image.height >= canvas.getHeight()) y = canvas.getHeight() - image.height;
}

void Object::draw(Window& canvas) {
    
   
    for (unsigned int i = 0; i < image.height; i++)
    {
        if ((y + i) >= 0 && (y + i) < canvas.getHeight()) {
            for (unsigned int n = 0; n < image.width; n++)
            {
                if ((x + n) >= 0 && (x + n) < canvas.getWidth())
                {
                    if (image.alphaAtUnchecked(n, i) > 210)
                    {
                        canvas.draw(x + n, y + i, image.atUnchecked(n, i));
                    }
                }
            }
        }
    } 
}
void Object::draw(Window& canvas,float _x,float _y) {


    for (unsigned int i = 0; i < image.height; i++)
    {
        if ((_y + i) >= 0 && (_y + i) < canvas.getHeight()) {
            for (unsigned int n = 0; n < image.width; n++)
            {
                if ((_x + n) >= 0 && (_x + n) < canvas.getWidth())
                {
                    if (image.alphaAtUnchecked(n, i) > 210)
                    {
                        canvas.draw(_x + n, _y + i, image.atUnchecked(n, i));
                    }
                }
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
Player::Player(float _x, float _y, int _speed, const std::string& path)
    : Object(_x, _y, _speed, path) {
    screenX = 512.0;
    screenY = 384.0;
    health = 100;
    attack = 20;
    score = 5;
}

void Player::drawPLayer(Window& canvas, Camera& cam) {
    float playerScreenX = x - cam.x;
    float playerScreenY = y - cam.y;

    // Clamp player's drawing position to the screen edges
    if (playerScreenX > canvas.getWidth() - image.width) {
        playerScreenX = canvas.getWidth() - image.width;
    }
    if (playerScreenX < 0) {
        playerScreenX = 0;
    }
    if (playerScreenY > canvas.getHeight() - image.height) {
        playerScreenY = canvas.getHeight() - image.height;
    }
    if (playerScreenY < 0) {
        playerScreenY = 0;
    }
    screenX = playerScreenX;
    screenY = playerScreenY;
    // Draw the player image on the canvas
    for (unsigned int i = 0; i < image.height; i++) {
        for (unsigned int n = 0; n < image.width; n++) {
            if ((screenX + n) >= 0 && (screenX + n) < canvas.getWidth()) {
                if (image.alphaAtUnchecked(n, i) > 210) {
                    canvas.draw(screenX + n, screenY + i, image.atUnchecked(n, i));
                }
            }
        }
    }
}
void Player::updatePlayer(float _x, float _y, int** mapData) {
    // Calculate potential new positions
    float newX = x + _x;
    float newY = y + _y;

    // Calculate tile indices for boundary checks
    const int mapWidth = 42 * 32;
    const int mapHeight = 42 * 32;

    // Ensure the player stays within the map bounds
    if (newX < 0) newX = 0;
    if (newY < 0) newY = 0;
    if (newX + image.width >= mapWidth) newX = mapWidth - image.width;
    if (newY + image.height >= mapHeight) newY = mapHeight - image.height;

    // Check for collisions before updating the position
    if (!checkCollision(mapData, newX, y)) {
        x = newX; // Move horizontally if no collision
    }
    if (!checkCollision(mapData, x, newY)) {
        y = newY; // Move vertically if no collision
    }
}

bool Player::checkCollision(int** mapData, float newX, float newY) {
    // Calculate the center of the player
    int centerX = static_cast<int>(newX + image.width / 2 - 1);
    int centerY = static_cast<int>(newY + image.height / 2 + 15);

    // Convert center coordinates to tile indices
    int tileX = centerX / 32; // Assuming TILE_SIZE is 32
    int tileY = centerY / 32;

    // Ensure tile indices are within bounds
    if (tileX < 0 || tileX >= 42 || tileY < 0 || tileY >= 42) {
        return false; // No collision if out of bounds
    }

    // Get the tile ID at the center position
    int tileID = mapData[tileY][tileX];

    // Check if the tile ID is between 14 and 22 (blocked tiles)
    return (tileID >= 14 && tileID <= 22);
}
void Player::respawn() {
    // Implement respawn functionality here, if needed
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
Enemy::Enemy() {}

Enemy::Enemy(float _x, float _y, int _speed,  string path)
    : Object(_x, _y, _speed, path) {
    
}
   
// Move enemy toward the player
void Enemy::updateEnemy(Window& canvas, Player& player, float dt) {
    // Calculate direction vector toward the player
    float directionX = player.screenX - x;
    float directionY = player.screenY - y;

    

    // Calculate the magnitude (distance) of the direction vector
    float magnitude = std::sqrt(directionX * directionX + directionY * directionY);

    // Ensure we don't divide by zero
    if (magnitude > 0.0f) {
        // Normalize the direction vector
        float normX = directionX / magnitude;
        float normY = directionY / magnitude;

        // Move the enemy towards the player at a constant speed
        float move_amount = speed * dt;

        x += normX * move_amount;
        y += normY * move_amount;

       /* cout << "move_amount: " << move_amount << endl;
        cout << "normX: " << normX << " normY: " << normY << endl;
        cout << "New Position -> x: " << x << " y: " << y << endl;*/
    }

    
  
}






// Check for collision with the player
bool Enemy::checkCollision(Player& player) {
    return (x < player.screenX + player.image.width - 10 &&
        x + image.width >player.screenX + 10 &&
        y < player.screenY + player.image.height - 10 &&
        y + image.height > player.screenY + 10);
}




BatEnemy::BatEnemy() {}

BatEnemy::BatEnemy(float _x, float _y, int _speed, string path)
    : Enemy(_x, _y, _speed, path) {
}
BatEnemy::BatEnemy(float _x, float _y) : Enemy(_x, _y, 200, "Resources/bat.png") {}

DogEnemy::DogEnemy() {}

DogEnemy::DogEnemy(float _x, float _y, int _speed, string path)
    : Enemy(_x, _y, _speed, path) {
}
DogEnemy::DogEnemy(float _x, float _y) : Enemy(_x, _y, 100, "Resources/dog.png") {}

scorpionEnemy::scorpionEnemy() {}

scorpionEnemy::scorpionEnemy(float _x, float _y, int _speed, string path)
    : Enemy(_x, _y, _speed, path) {
}
scorpionEnemy::scorpionEnemy(float _x, float _y) : Enemy(_x, _y, 100, "Resources/scorpion.png") {}




Swarm::Swarm() {
    
}

// Update all enemies
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

        switch (enemyType) {
        case 0:
            enemies.push_back(new BatEnemy(spawnX, spawnY));
            break;
        case 1:
            enemies.push_back(new DogEnemy(spawnX, spawnY));
            break;
        case 2:
            enemies.push_back(new scorpionEnemy(spawnX, spawnY));
            break;
        
        }

        // Add the new enemy to the list
        
        

        //cout << "ENEMY CREATED at (" << spawnX << ", " << spawnY << ")" << endl;
        
        timeElapsed = 0;
    }

    // Update each enemy and check for collisions
    for (auto it = enemies.begin(); it != enemies.end(); ) {
        
        if (player.screenX > 511.0&& player.screenX <513.0) {
            (*it)->x -= playerX;
        }
        if (player.screenY > 383.0 && player.screenY < 385.0) {
            (*it)->y -= playerY;
        }
        (*it)->updateEnemy(canvas, player, dt);
        //cout << enemies.at(0).x << endl;
        if ((*it)->checkCollision(player)) {
            cout << "ENEMY COLLIDED WITH PLAYER" << endl;
            delete* it;
            it = enemies.erase(it); // Safe removal
        }
        else {
            ++it;
        }
        
    }
}


// Draw all enemies
void Swarm::draw(Window& canvas) {
    for (auto& enemy : enemies) {
        enemy->draw(canvas);
    }
}
Swarm::~Swarm() {
    // Delete all dynamically allocated enemies to avoid memory leaks
    for (auto enemy : enemies) {
        delete enemy;
    }
    enemies.clear();  // Clear the vector
}