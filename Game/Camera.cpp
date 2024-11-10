
#include "Camera.h"




void Camera::update(int playerX, int playerY) {
    // Center camera around player
    x = playerX - windowWidth / 2;
    y = playerY - windowHeight / 2;

    // Clamp the camera position within the bounds of the map
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x > mapWidth - windowWidth) x = mapWidth - windowWidth;
    if (y > mapHeight - windowHeight) y = mapHeight - windowHeight;
}