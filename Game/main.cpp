#include "Object.h"
#include"World.h"
#include <memory>
using namespace std;
int main() {
	// Create a canvas window with dimensions 1024x768 and title �Tiles"
	GamesEngineeringBase::Window canvas;
	canvas.create(1024, 768, "Game");
	bool running = true; // Variable to control the main loop's running state.



	unique_ptr<Object> player = make_unique<Player>(400, 300, Round,32,0,0,0);
	

	World w;


	Timer timer;

	float MOVE_SPEED = 600.0f;
	while (running)
	{
		// Check for input (key presses or window events)
		canvas.checkInput();
		// Clear the window for the next frame rendering
		canvas.clear();
		// Update game logic
		
		if (canvas.keyPressed(VK_ESCAPE))
		{
			break; // Exits the game loop
		}

		// Get the delta time for the frame
		float dt = timer.dt();
		unsigned int move_amount = static_cast<unsigned int>(max(MOVE_SPEED * dt, 1.0f));
		int playerX = 0;
		int playerY = 0;
		
		if (canvas.keyPressed('W')) // Move plane up
		{
			playerY -= move_amount;// Speed is adjusted based on time delta.
		}
		if (canvas.keyPressed('S')) // Move plane down
		{
			playerY += move_amount;
		}
		if (canvas.keyPressed('A')) // Move plane left
		{
			playerX -= move_amount;
		}
		if (canvas.keyPressed('D')) // Move plane right
		{
			playerX += move_amount;
		}
		
		// Update()
		player->update(canvas, playerX, playerY);
		//enemy.update(canvas,dt);



		// Draw();
		for (unsigned int i = 0; i < (canvas.getWidth() * canvas.getHeight()); i++)
		{
			canvas.draw(i, 255, 255, 255); 
		}
		
		w.draw(canvas, 0);
		
		player->draw(canvas);
		//enemy.draw(canvas);
		canvas.present();
	}
	

	return 0;
}
