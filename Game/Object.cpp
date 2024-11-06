#include "Object.h"

Object::Object(int _x, int _y, string path) {
    x = _x;
    y = _y;
    image.load(path);
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
    if (x + image.width >= canvas.getWidth()) x = canvas.getWidth() - image.width;
    if (y + image.height >= canvas.getHeight()) y = canvas.getHeight() - image.height;
}

void Object::draw(Window& canvas) {
    cout << image.height;
    cout << image.width << endl;
   
    for (unsigned int i = 0; i < image.height; i++)
    {
        for (unsigned int n = 0; n < image.width; n++)
        {

            if ((x + n) >= 0 && (x + n) < canvas.getWidth() && (y + i) >= 0 && (y + i) < canvas.getHeight())
            {
                if (image.alphaAt(n, i) > 210)
                {
                    canvas.draw(x + n, y + i, image.at(n, i));
                }
            }
        }
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