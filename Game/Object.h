#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "GamesEngineeringBase.h"
#include"Camera.h"
#include"Tile.h"
#include <string>
#include <iostream>

#include<math.h>

using namespace GamesEngineeringBase;
using namespace std;

class Swarm;

class Object {

public:
    Image image;
    float x, y;
    int speed;
    
    Object();
    Object(float _x, float _y,int _speed, const string& path);
    void update(float _x, float _y);
    void update(Window& canvas, float _x, float _y);
    void draw(Window& canvas);
    void draw(Window& canvas, float _x, float _y);
    

};
class Fireball :public Object {
    float directionX, directionY;
public:
    bool isActive;
    bool isBig;
    // Constructor to initialize a Fireball object with its position, direction, and speed
    Fireball(float _x, float _y, float _directionX, float _directionY, int _speed, const string& path, bool _isBig);

    // Update the fireball's position
    void update(float dt, bool moveX,bool moveY, int playerX, int playerY);

    // Check for collisions with enemies



};

class Player : public Object {
public:
    int health;
    int attack;
    int score;
    float screenX, screenY;
    float attackSpeed;
    int fireballCount;           // To track how many fireballs are active
    int fireballMax;             // Max number of fireballs the player can shoot at once
    float fireballCooldown;
    float bigFireballCooldown;
    Fireball** fireballs;
    bool moveX;
    bool moveY;


    Player(float _x, float _y, int _speed, const std::string& path);
    ~Player();
    void drawPLayer(Window& canvas);
    void updatePlayer(float _x, float _y, int** mapData);
    bool checkCollision(int** mapData, float newX, float newY);
    void reduceHealth(int attack);
    void shootFireball(Swarm& swarm, float dt, int playerX, int playerY, bool bigFireBall);
    void addFireBall(Fireball* fireball);
    void removeFireBall(int index);
    void aoeCollision(Swarm& swarm, Fireball& fireball);
    void drawFireballs(Window& canvas);
};


class Enemy : public Object {
public:
    int health;
    int attack;

   
    bool canShootFireballs;
    Enemy();
    virtual ~Enemy() {}
    Enemy(float _x, float _y, int _speed,  string path);
    void updateEnemy(Window& canvas, Player& player, float dt);
    
    bool checkCollision(Player& player);
};

class BatEnemy : public Enemy {
public:
    BatEnemy();
    BatEnemy(float _x, float _y);
};
class DogEnemy : public Enemy {
public:
    DogEnemy();
    DogEnemy(float _x, float _y);
};
class scorpionEnemy : public Enemy {
public:
    scorpionEnemy();
    scorpionEnemy(float _x, float _y);
};

class fireMonsterEnemy : public Enemy {
public:
    Fireball** fireballs; // Array to store fireballs
    int fireballCount;
    int fireballMax;
    float fireballCooldown;
    fireMonsterEnemy();
    ~fireMonsterEnemy();
    fireMonsterEnemy(float _x, float _y);
    void shootFireball(Player& player, float dt);
    void updateFireballs(Window& canvas, Player& player, float dt, int playerX, int playerY);
    void removeFireBall(int index);
    void drawFireballs(Window& canvas);
};






class Swarm {
private:
    Enemy** enemies;// Dynamic array of Enemy pointers
    int maxEnemies;  // Max number of enemies in the swarm
    int numEnemies;  // Current number of enemies
    float timeElapsed = 0.0f;
    float spawnInterval = 2.0f; // Spawn a new enemy every 2 seconds
    float  totalElapsedTime = 0.0f;

public:
    
    Swarm();
    ~Swarm();
    void update(Window& canvas, Player& player, float dt, float playerX, float playerY);
    void draw(Window& canvas);
    void addEnemy(Enemy* enemy);  // Method to add a new enemy
    void removeEnemy(int index, Player& player);  // Method to remove an enemy by index
    void reduceHealth(int index, Player& player);
    int getNumEnemies() const;  // Getter for the current number of enemies
    Enemy** getEnemies() const;
    void checkFireballCollisions(Player& player);
};

#endif // OBJECT_H
