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
    screenX = 512.0f;
    screenY = 384.0f;
    health = 100;
    attack = 50;
    score = 0;
    fireballCooldown = 0.0f;
    bigFireballCooldown = 0.0f;
    fireballMax = 50;
    fireballCount = 0;
    attackSpeed = 1.5f;
    moveX = false;
    moveY = false;
    // Initialize dynamic array for fireballs
    fireballs = new Fireball * [fireballMax];
}
Player::~Player() {
    // Delete the fireballs dynamically allocated
    for (int i = 0; i < fireballCount; i++) {
        delete fireballs[i];
    }
    delete[] fireballs;
}

void Player::drawPLayer(Window& canvas) {
    
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
    moveX = false;
    moveY = false;
    // Calculate potential new positions
    float newX = x + _x;
    float newY = y + _y;

    // Calculate tile indices for boundary checks
    const int mapWidth = 72 * 32;
    const int mapHeight =72 * 32;

    // Ensure the player stays within the map bounds
    if (newX < 0) newX = 0;
    if (newY < 0) newY = 0;
    if (newX + image.width >= mapWidth) newX = mapWidth - image.width;
    if (newY + image.height >= mapHeight) newY = mapHeight - image.height;


    // Check for collisions before updating the position
    if (!checkCollision(mapData, newX, y)) {
        x = newX; // Move horizontally if no collision
        moveX = true;
    }
    if (!checkCollision(mapData, x, newY)) {
        y = newY; // Move vertically if no collision
        moveY = true;
    }

    
}
void Player::reduceHealth(int attack) {
    health -= attack;
}
bool Player::checkCollision(int** mapData, float newX, float newY) {
    // Calculate the center of the player
    int centerX = static_cast<int>(newX + image.width / 2 );
    int centerY = static_cast<int>(newY + image.height / 2 +15);

    // Convert center coordinates to tile indices
    int tileX = centerX / 32; // Assuming TILE_SIZE is 32
    int tileY = centerY / 32;

    // Ensure tile indices are within bounds
    if (tileX < 0 || tileX >= 72 || tileY < 0 || tileY >= 72) {
        return false; // No collision if out of bounds
    }

    // Get the tile ID at the center position
    int tileID = mapData[tileY][tileX];

    // Check if the tile ID is between 14 and 22 (blocked tiles)
    return (tileID >= 14 && tileID <= 22);
}

void Player::shootFireball(Swarm& swarm, float dt,int playerX,int playerY,bool bigFireBall) {
    fireballCooldown += dt;
    bigFireballCooldown += dt;
    if ((fireballCooldown >= attackSpeed && fireballCount < fireballMax)||(bigFireballCooldown>=5.0f&&bigFireBall)) {
        // Find the closest enemy
        Enemy* closestEnemy = nullptr;
        float closestDistance = 2000.0f;

        for (int i = 0; i < swarm.getNumEnemies(); i++) {
            Enemy* enemy = swarm.getEnemies()[i];
            float distance = sqrt(pow(enemy->x - screenX, 2) + pow(enemy->y - screenX, 2));
            if (distance < closestDistance) {
                closestDistance = distance;
                closestEnemy = enemy;
            }
        }

        // If a closest enemy is found, shoot a fireball toward it
        if (closestEnemy) {
            // Calculate direction to the enemy
            float directionX = closestEnemy->x - screenX;
            float directionY = closestEnemy->y - screenY;
            float magnitude = sqrt(directionX * directionX + directionY * directionY);
            directionX /= magnitude;  // Normalize direction
            directionY /= magnitude;

            Fireball* fireball = nullptr;
            if (bigFireBall&& bigFireballCooldown>=5.0f) {
                fireball = new Fireball(screenX, screenY, directionX, directionY, 500, "Resources/bigFireBall.png", true);
                bigFireballCooldown = 0.0f;
                fireballCooldown = 0.0f;
                
            }
            else {
                fireball = new Fireball(screenX, screenY, directionX, directionY, 500, "Resources/smallFireBall.png", false);
                fireballCooldown = 0.0f;
            }
            
            addFireBall(fireball);
           
        }

       
    }
    for (int i = 0; i < fireballCount; ) {
        Fireball* currentFireball = fireballs[i];
        currentFireball->update(dt,moveX,moveY,playerX,playerY);

        if (!currentFireball->isActive) {
            if (currentFireball->isBig) {
                aoeCollision(swarm, *currentFireball);
            }
            removeFireBall(i);  // Remove enemy from array
        }
        else {
            ++i;
        }
    }
}
void Player::aoeCollision(Swarm& swarm, Fireball& fireball) {
    const float aoeRadius = 200.0f;
    for (int i = 0; i < swarm.getNumEnemies(); ) {
        Enemy* enemy = swarm.getEnemies()[i];
        float distance = sqrt(pow(enemy->x - fireball.x, 2) + pow(enemy->y - fireball.y, 2));
        if (distance <= aoeRadius) {
            // Kill the enemy within AoE range
            swarm.reduceHealth(i,*this);
        }
        else {
            ++i;
        }
    }
}
void Player::drawFireballs(Window& canvas) {
    for (int i = 0; i < fireballCount; ++i) {
        if (fireballs[i]->isActive) {
            fireballs[i]->draw(canvas);  
        }
    }
}
void Player::addFireBall(Fireball* fireball) {
    // Check if there is space for a new enemy
    if (fireballCount < fireballMax) {
        fireballs[fireballCount] = fireball;  // Add the new enemy to the array
        ++fireballCount;  // Increment the number of enemies
    }
    else {
        std::cerr << "Error: Cannot add more fireballs, max limit reached!" << std::endl;
    }
}

void Player::removeFireBall(int index) {
    if (index >= 0 && index < fireballCount) {
        delete fireballs[index];  // Delete the enemy at the given index
        // Shift remaining enemies to the left
        for (int i = index; i < fireballCount - 1; ++i) {
            fireballs[i] = fireballs[i + 1];
        }
        --fireballCount;  // Decrease the number of enemies
        
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
Enemy::Enemy() {}

Enemy::Enemy(float _x, float _y, int _speed,  string path)
    : Object(_x, _y, _speed, path) {
    canShootFireballs = false;
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


BatEnemy::BatEnemy(float _x, float _y) : Enemy(_x, _y, 200, "Resources/bat.png") {
    health = 50;
    attack = 10;
}

DogEnemy::DogEnemy() {}


DogEnemy::DogEnemy(float _x, float _y) : Enemy(_x, _y, 100, "Resources/dog.png") {
    health = 50;
    attack = 20;
}

scorpionEnemy::scorpionEnemy() {}

scorpionEnemy::scorpionEnemy(float _x, float _y) : Enemy(_x, _y, 80, "Resources/scorpion.png") {
    health = 100;
    attack = 20;
}
fireMonsterEnemy::fireMonsterEnemy() {}
fireMonsterEnemy::~fireMonsterEnemy() {
    for (int i = 0; i < fireballCount; i++) {
        delete fireballs[i];
    }
    delete[] fireballs;
}

fireMonsterEnemy::fireMonsterEnemy(float _x, float _y) : Enemy(_x, _y, 50, "Resources/fire.png") {
    health = 100;
    attack = 5;
    fireballMax = 5;
    fireballCount = 0;
    fireballCooldown = 0.0f;
    canShootFireballs = true;
    // Allocate memory for fireballs
    fireballs = new Fireball * [fireballMax];
}

void fireMonsterEnemy::shootFireball(Player& player, float dt ) {
    fireballCooldown += dt;

    
    if (fireballCooldown >= 2.0f && fireballCount < fireballMax) {
        // Calculate direction towards the player
        float directionX = player.screenX - x;
        float directionY = player.screenY - y;

        
        float magnitude = sqrt(directionX * directionX + directionY * directionY);
        if (magnitude > 0) {
            directionX /= magnitude;
            directionY /= magnitude;
        }
        cout << "fireballX " << x << "  fireballY  " << y << endl;
        // Create a new fireball aimed at the player
        Fireball* fireball = new Fireball(x, y, directionX, directionY, 200, "Resources/fireball.png", false);

        fireballs[fireballCount] = fireball;
        cout << "ADD bALL\n";
        fireballCount++;
        fireballCooldown = 0.0f;
    }
    
}

void fireMonsterEnemy::updateFireballs(Window& canvas, Player& player, float dt, int playerX, int playerY) {
    for (int i = 0; i < fireballCount;) {
        Fireball* fireball = fireballs[i];
        
        fireball->update(dt, player.moveX, player.moveY, playerX, playerY);

        // Check if the fireball hit the player
        if (fireball->x < player.screenX + player.image.width/2 &&
            fireball->x + fireball->image.width > player.screenX+ player.image.width / 2 &&
            fireball->y < player.screenY + player.image.height/2 &&
            fireball->y + fireball->image.height > player.screenY + player.image.height / 2) {
            
            player.reduceHealth(attack); // Damage player
            fireball->isActive = false;
        }
        if (!fireball->isActive) {
            removeFireBall(i);
        }
        else {
            ++i;
        }
    }
    

}
void fireMonsterEnemy::removeFireBall(int index) {
    if (index >= 0 && index < fireballCount) {
        delete fireballs[index];  // Delete the enemy at the given index
        // Shift remaining enemies to the left
        for (int i = index; i < fireballCount - 1; ++i) {
            fireballs[i] = fireballs[i + 1];
        }
        --fireballCount;  // Decrease the number of enemies

    }
}
void fireMonsterEnemy::drawFireballs(Window& canvas) {
    //cout << "fireballCount "<< fireballCount<<endl;
    for (int i = 0; i < fireballCount; ++i) {
        if (fireballs[i]->isActive) {
            
            fireballs[i]->draw(canvas);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Fireball::Fireball(float _x, float _y, float _directionX, float _directionY, int _speed, const string& path,bool _isBig)
    : Object(_x, _y, _speed, path) {
    directionX = _directionX;
    directionY = _directionY;
    x = _x;
    y = _y;
    isActive = true;
    isBig = _isBig;
    
}
void Fireball::update(float dt, bool moveX, bool moveY,int playerX, int playerY) {
    if (isActive) {
        if (moveX) {
            x -= playerX;
        }
        if (moveY) {
            y -= playerY;
        }
        x += directionX * speed * dt;
        y += directionY * speed * dt;

        // If the fireball moves off the screen, deactivate it
        if (x < 0 || x > 1024 || y < 0 || y > 768) {  // assuming 800x600 screen size
            isActive = false;
        }
    }
}






/// <summary>
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// </summary>
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

void Swarm::removeEnemy(int index,Player& player) {
    if (index >= 0 && index < numEnemies) {
        delete enemies[index];  // Delete the enemy at the given index
        // Shift remaining enemies to the left
        for (int i = index; i < numEnemies - 1; ++i) {
            enemies[i] = enemies[i + 1];
        }
        --numEnemies;  // Decrease the number of enemies
        player.score++;
    }
}
void Swarm::reduceHealth(int index,Player& player) {
    enemies[index]->health -= player.attack;
    if (enemies[index]->health <= 0) {
        removeEnemy(index,player);
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
    totalElapsedTime += dt;
    if (totalElapsedTime >= 10.0f) {
        totalElapsedTime = 0; // Reset the timer for the next interval
        spawnInterval = max(0.1f, spawnInterval * 0.9f); // Reduce spawn interval by 10%, minimum of 0.5 seconds
        player.speed = min(400, player.speed * 1.3f);
        player.attackSpeed = max(0.5, player.attackSpeed * 0.9f);
    }
    // Spawn a new enemy if enough time has passed
    if (timeElapsed > spawnInterval) {
        // Determine random spawn location outside the screen
        int spawnX, spawnY;

        // Randomly choose one of four sides of the screen for spawning
        int side = rand() % 4;
        int enemyType = rand() % 6;
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
            newEnemy = new BatEnemy(spawnX, spawnY);
            break;
        case 2:
            newEnemy = new DogEnemy(spawnX, spawnY);
            break;
        case 3:
            newEnemy = new DogEnemy(spawnX, spawnY);
            break;
        case 4:
            newEnemy = new scorpionEnemy(spawnX, spawnY);
            break;
        case 5:
            newEnemy = new fireMonsterEnemy(spawnX, spawnY);
            break;
        }

        if (newEnemy != nullptr) {
            addEnemy(newEnemy);  // Add the newly created enemy
        }

        timeElapsed = 0;
    }

    // Update each enemy and check for collisions
    for (int i = 0; i < numEnemies;) {
        Enemy* currentEnemy = enemies[i];
        if (player.moveX) currentEnemy->x -= playerX;
        if (player.moveY) currentEnemy->y -= playerY;

        currentEnemy->updateEnemy(canvas, player, dt);

        // fireMonsterEnemy shoots fireball
        if (currentEnemy->canShootFireballs) {
            fireMonsterEnemy* fireMonster = static_cast<fireMonsterEnemy*>(currentEnemy);
            fireMonster->shootFireball(player, dt);
            fireMonster->updateFireballs(canvas, player, dt,playerX,playerY);
          
        }

        if (currentEnemy->checkCollision(player)) {
            player.reduceHealth(currentEnemy->attack);
            removeEnemy(i,player);
            player.score--;
        }
        else {
            ++i;
        }
    }
}

void Swarm::checkFireballCollisions(Player& player) {
    for (int i = 0; i < numEnemies; ) {
        Enemy* currentEnemy = enemies[i];
        bool enemyHit = false;

        // Check collision with all active fireballs
        for (int j = 0; j < player.fireballCount; ) {
            Fireball* fireball = player.fireballs[j];

            // Check if the fireball is active
            if (fireball->isActive) {
                // Perform AABB (Axis-Aligned Bounding Box) collision detection
                if (fireball->x < currentEnemy->x + currentEnemy->image.width &&
                    fireball->x + fireball->image.width > currentEnemy->x &&
                    fireball->y < currentEnemy->y + currentEnemy->image.height &&
                    fireball->y + fireball->image.height > currentEnemy->y) {

                    // Collision detected
                    std::cout << "FIREBALL HIT ENEMY" << std::endl;

                    // Deactivate the fireball
                    fireball->isActive = false;

                    // Remove the enemy
                    reduceHealth(i,player);
                    enemyHit = true;
                    break;
                }
            }
            // Move to the next fireball if no collision
            if (!enemyHit) {
                ++j;
            }
        }

        // If the enemy was hit, don't increment 'i' since removeEnemy shifts the array
        if (!enemyHit) {
            ++i;
        }
    }
}



void Swarm::draw(Window& canvas) {
    for (int i = 0; i < numEnemies; ++i) {
        enemies[i]->draw(canvas);  // Draw each enemy in the array

        if (enemies[i]->canShootFireballs) { // Check if the enemy can shoot fireballs
            
            fireMonsterEnemy* fireMonster = dynamic_cast<fireMonsterEnemy*>(enemies[i]);

            if (fireMonster != nullptr) { // Check if the dynamic cast was successful
                
                fireMonster->drawFireballs(canvas);
            }
            else {
                cout << "Error: dynamic_cast to fireMonsterEnemy failed.\n";
            }
        }
    }
}
