#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "GamesEngineeringBase.h"
#include <string>
#include <iostream>

using namespace GamesEngineeringBase;
using namespace std;

enum Shape {
    Round,Square
};

class Object {
public:
    int x, y;
    Shape shape;
    int size;
    int color[3];
    int r, g, b;

    Object(int _x, int _y,Shape _shape, int _size,int _r,int _g,int _b);
    void update(int _x, int _y);
    void update(Window& canvas, int _x, int _y);
    void draw(Window& canvas);
};

class Player : public Object {
public:
    Player(int _x, int _y,Shape _shape, int _size, int _r, int _g, int _b) : Object(_x, _y, _shape,_size,_r,_g,_b) {}
    void respawn();
};

#endif // OBJECT_H
