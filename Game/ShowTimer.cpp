#include"ShowTimer.h"

ShowTimer::ShowTimer() : elapsedTime(0.0f) {}


void ShowTimer::load() {
    for (int i = 0; i < 10; i++) {
        // Load each tile from the Resources folder with a filename based on index
        string t = "Resources/text/t" + to_string(i) + ".png";
        timeNumbers[i].image.load(t);
        string h = "Resources/text/h" + to_string(i) + ".png";
        healthNumbers[i].image.load(h);
        string s = "Resources/text/s" + to_string(i) + ".png";
        scoreNumbers[i].image.load(s);
        string f = "Resources/text/" + to_string(i) + ".png";
        finalScoreNumbers[i].image.load(f);
    }
}

void ShowTimer::update(float dt, int _health, int _score) {
    health = _health;
    score = _score;
    elapsedTime += dt;  // Update the elapsed time with delta time (dt)
    fps = 0;
}


void ShowTimer::drawText(Window& canvas, Object obj[], int number, int screenX,int screenY) {
    int temp = number;
    int numDigits = 0;

    // Count the number of digits
    while (temp > 0) {
        numDigits++;
        temp /= 10;
    }

    // Dynamically allocate an array to store digits
    int* digits = new int[numDigits];
    int index = numDigits - 1;
    temp = number;

    // Extract digits from the number
    while (temp > 0) {
        digits[index] = static_cast<int>(temp) % 10;
        temp /= 10;
        index--;
    }

    // Draw each digit using the provided 'obj' array
    for (int i = 0; i < numDigits; ++i) {
        obj[digits[i]].draw(canvas, screenX, screenY); // Use 'obj' instead of 'timeNumbers'
        screenX += 24;
       
    }

    // Free the allocated memory
    delete[] digits;
}

void ShowTimer::draw(Window& canvas) {
    int time = static_cast<int>(elapsedTime);

    drawText(canvas, timeNumbers, time, canvas.getWidth() / 2,5); // Draw elapsed time
    drawText(canvas, healthNumbers, health, 0,5);                // Draw health
    drawText(canvas, scoreNumbers, score, 900,5);               // Draw score
    drawText(canvas, scoreNumbers, fps, 900,40);
}

