#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "GamesEngineeringBase.h"
#include <string>
#include <iostream>

using namespace GamesEngineeringBase;
using namespace std;

class Object {
public:
    int x, y;
    Image image;

    Object(int _x, int _y, string path);
    void update(int _x, int _y);
    void update(Window& canvas, int _x, int _y);
    void draw(Window& canvas);
};

class Player : public Object {
public:
    Player(int _x, int _y, string path) : Object(_x, _y, path) {}
    void respawn();
};

#endif // OBJECT_H
