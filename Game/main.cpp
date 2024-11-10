#include "Object.h"
#include"World.h"
#include"Camera.h"
#include "ShowTimer.h"
#include <memory>
using namespace std;
int main() {
	// Create a canvas window with dimensions 1024x768 and title “Tiles"
	GamesEngineeringBase::Window canvas;
	canvas.create(1024, 768, "Game");
	bool running = true; // Variable to control the main loop's running state.

	

	unique_ptr<Player> player = make_unique<Player>(512.0, 384.0,500,"Resources/player.png");
	unique_ptr<Camera> cam = make_unique<Camera>(canvas.getWidth(), canvas.getHeight(), 1344, 1344);
	unique_ptr<World> world = make_unique<World>();
	
	unique_ptr<ShowTimer> showTimer = make_unique<ShowTimer>();
	showTimer->load();
	
	Swarm swarm;

	

	Object startMenu;
	startMenu.image.load("Resources/menu.png");

	while (running) {
		canvas.clear();
		canvas.checkInput();

		if (canvas.keyPressed('E'))break;
		if (canvas.keyPressed('W')) {
			world->surviveMode = false;
			break;
		}
		if (canvas.keyPressed('Q'))running=false;
		startMenu.draw(canvas);
		canvas.present();
	}
	
	world->load();
	Timer timer;
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
		showTimer->update(dt,player->health,player->score);
		float move_amount = static_cast<float>(max(player->speed * dt, 1.0f));
		float playerX = 0;
		float playerY = 0;
		
		if (canvas.keyPressed('W')) playerY -= move_amount;
		if (canvas.keyPressed('S')) playerY += move_amount;
		if (canvas.keyPressed('A')) playerX -= move_amount;
		if (canvas.keyPressed('D')) playerX += move_amount;
		if (canvas.keyPressed('Q'))running = false;
		// Update()
		player->updatePlayer(playerX, playerY,world->getMapData());

		// Update the camera based on the player's new position
		cam->update(player->x, player->y);
		swarm.update(canvas, *player, dt, playerX, playerY);
		// Draw the map with respect to the camera's position
		world->drawMap(canvas, cam->x, cam->y);

		// Draw the player with camera offset
		player->drawPLayer(canvas, *cam);
		swarm.draw(canvas);
		showTimer->draw(canvas);
		canvas.present();
	}
	

	return 0;
}
