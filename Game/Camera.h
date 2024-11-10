#pragma once
// Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include "GamesEngineeringBase.h"
using namespace GamesEngineeringBase;


const int PLAYER_SIZE = 64;
class Camera {
public:
    int x, y;
    int windowWidth, windowHeight;
    int mapWidth, mapHeight;

    Camera(int windowW, int windowH, int mapW, int mapH)
        : x(0), y(0), windowWidth(windowW), windowHeight(windowH),
        mapWidth(mapW), mapHeight(mapH) {}

    void update(int playerX, int playerY);
};

#endif // CAMERA_H
