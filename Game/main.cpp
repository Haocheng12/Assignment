#include "Object.h"
#include"World.h"
#include"Camera.h"
#include <memory>
using namespace std;
int main() {
	// Create a canvas window with dimensions 1024x768 and title “Tiles"
	GamesEngineeringBase::Window canvas;
	canvas.create(1024, 768, "Game");
	bool running = true; // Variable to control the main loop's running state.

	const int MAP_WIDTH = 1344; // Example map width
	const int MAP_HEIGHT = 1344; // Example map height

	unique_ptr<Player> player = make_unique<Player>(512.0, 384.0,500,"Resources/player.png");
	unique_ptr<Camera> cam = make_unique<Camera>(canvas.getWidth(), canvas.getHeight(), MAP_WIDTH, MAP_HEIGHT);
	unique_ptr<World> world = make_unique<World>();
	world->load();
	Swarm swarm;

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
		float move_amount = static_cast<float>(max(player->speed * dt, 1.0f));
		float playerX = 0;
		float playerY = 0;
		
		if (canvas.keyPressed('W')) playerY -= move_amount;
		if (canvas.keyPressed('S')) playerY += move_amount;
		if (canvas.keyPressed('A')) playerX -= move_amount;
		if (canvas.keyPressed('D')) playerX += move_amount;
		
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
		canvas.present();
	}
	

	return 0;
}
