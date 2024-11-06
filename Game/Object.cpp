#include "Object.h"

Object::Object(int _x, int _y, Shape _shape, int _size, int _r, int _g, int _b) {
    x = _x;
    y = _y;
    shape = _shape;
    size = _size;
    r = _r;
    g = _g;
    b = _b;
    
}
void Object::update(int _x, int _y) {
    x += _x;
    y += _y;
}

void Object::update(Window& canvas, int _x, int _y) {
    x += _x;
    y += _y;
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + size >= canvas.getWidth()) x = canvas.getWidth() - size;
    if (y + size >= canvas.getHeight()) y = canvas.getHeight() - size;
}

void Object::draw(Window& canvas) {
    
    switch (shape) {
    case Round: {
        int radius = size / 2;
        for (int i = 0; i <= size; i++) {
            for (int j = 0; j <= size; j++) {
                // Check if the point (j, i) lies within the circle with radius^2
                if ((i - radius)* (i - radius) + (j - radius) * (j - radius) <= radius * radius) {
                    int drawX = x + j;
                    int drawY = y + i;

                    // Ensure the point is within canvas bounds
                    if (drawX >= 0 && drawX < canvas.getWidth() && drawY >= 0 && drawY < canvas.getHeight()) {
                        canvas.draw(drawX, drawY, r, g, b);
                    }
                }
            }
        }
        break;
    }
    case Square: {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if ((x + j) >= 0 && (x + j) < canvas.getWidth() && (y + i) >= 0 && (y + i) < canvas.getHeight())
                {

                    canvas.draw(x + j, y + i, r, g, b);

                }
            }
        }
    }break;
    }
    
    

    
}

void Player::respawn() {
    // Implement respawn functionality here, if needed
}


//class Sworm {
//
//	Plane* enemyArray[100];
//	unsigned int currentSize = 0;
//	float timeElapsed = 0;
//public:
//	
//
//	Sworm() {
//		for (int i = 0; i < 100; i++) {
//			enemyArray[i] = nullptr;  
//		}
//	}
//
//	
//	void update(Window& canvas, float dt) {
//		timeElapsed += dt;
//
//		// Check if it's time to add a new plane
//		if (currentSize < 100 && timeElapsed > 2.f) {
//			enemyArray[currentSize] = new Plane(300, -200, "Resource/L2.png");
//			currentSize++;
//			timeElapsed = 0;
//		}
//
//		// Move the planes
//		unsigned int move_amount = static_cast<unsigned int>(max(1000.f * dt, 3.0f));
//
//
//		for (int i = 0; i < currentSize;i++) {
//			if (enemyArray[i]!= nullptr) {
//				enemyArray[i]->update(0, 3);
//			}
//		}
//	}
//
//	void draw(Window& canvas) {
//		
//		for (Plane* ptr : enemyArray) {
//			if (ptr != nullptr) {
//				ptr->draw(canvas);
//			}
//		}
//	}
//};