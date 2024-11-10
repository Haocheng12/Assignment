#pragma once
#ifndef TIMER_H
#define TIMER_H

#include<iostream>
#include <string>
#include "GamesEngineeringBase.h"// Assuming you have a Window class for drawing
#include"Object.h"
using namespace GamesEngineeringBase;

class ShowTimer {
private:
    float elapsedTime;  // Time in seconds
    int health;
    int score;
    Object timeNumbers[10];
    Object healthNumbers[10];
    Object scoreNumbers[10];
    Object finalScoreNumbers[10];

public:
    // Constructor
    ShowTimer();
    void load();
    // Update the timer by the given delta time (dt)
    void update(float dt,int _health,int _score);
    
    void drawText(Window& canvas, Object obj[], int number, int screenX);
    // Draw the timer to the screen at the top-left corner
    void draw(Window& canvas);
};

#endif // TIMER_H
