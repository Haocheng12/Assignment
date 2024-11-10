#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "GamesEngineeringBase.h"
#include"Camera.h"
#include"Tile.h"
#include <string>
#include <iostream>
#include<vector>
#include<math.h>

using namespace GamesEngineeringBase;
using namespace std;


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
    

};

class Player : public Object {
public:
    float screenX, screenY;
    Player(float _x, float _y, int _speed, const std::string& path);
    void drawPLayer(Window& canvas, Camera& cam);
    void updatePlayer(float _x, float _y, int** mapData);
    bool checkCollision(int** mapData, float newX, float newY);
    void respawn();
};


class Enemy : public Object {
public:
    Enemy();
    Enemy(float _x, float _y, int _speed,  string path);
    void updateEnemy(Window& canvas, Player& player, float dt);
    bool checkCollision(Player& player);
};

class BatEnemy : public Enemy {
public:
    BatEnemy();
    BatEnemy(float _x, float _y);
    BatEnemy(float _x, float _y, int _speed, string path);
    
};



class Swarm {
    
    vector<Enemy*> enemies;
    float timeElapsed = 0.0f;
    float spawnInterval = 2.0f; // Spawn a new enemy every 2 seconds

public:
    Swarm();
    ~Swarm();
    void update(Window& canvas, Player& player, float dt, float playerX, float playerY);
    void draw(Window& canvas);
};

#endif // OBJECT_H
