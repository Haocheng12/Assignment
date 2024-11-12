#include "Object.h"
#include"World.h"
#include"Camera.h"
#include "ShowTimer.h"
#include "SaveLoad.h"
#include <memory>
using namespace std;
int main() {
	// Create a canvas window with dimensions 1024x768 and title “Tiles"
	GamesEngineeringBase::Window canvas;
	canvas.create(1024, 768, "Game");
	bool running = true; // Variable to control the main loop's running state.

	

	unique_ptr<Player> player = make_unique<Player>(1000.0, 600.0,120,"Resources/player.png");
	unique_ptr<World> world = make_unique<World>();
	unique_ptr<ShowTimer> showTimer = make_unique<ShowTimer>();
	
	showTimer->load();
	
	
	
	float totalTime = showTimer->elapsedTime;
	float frameTime = 0.0f;  // Time accumulated over frames
	int frameCount = 0;      // Frame counter
	float fps = 0.0f;

	Object startMenu;
	startMenu.image.load("Resources/menu.png");

	while (running) {
		canvas.clear();
		canvas.checkInput();

		if (canvas.keyPressed('E'))break;
		if (canvas.keyPressed('W')) {
			world->surviveMode = false;
			player->x = 35 * 32 / 2;
			player->y = 35 * 32 / 2;
			break;
		}
		if (canvas.keyPressed('Q'))running=false;
		startMenu.draw(canvas);

		canvas.present();
	}
	Swarm swarm;
	bool loaded = SaveLoad::loadGame(*player, *world, *showTimer, swarm);

	
	cout << loaded << endl;
	world->load(loaded);
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
		
		bool bigFireBall = false;
		if (canvas.keyPressed('W')) playerY -= move_amount;
		if (canvas.keyPressed('S')) playerY += move_amount;
		if (canvas.keyPressed('A')) playerX -= move_amount;
		if (canvas.keyPressed('D')) playerX += move_amount;
		if (canvas.keyPressed('E')) bigFireBall =true;
		
		// Update()
		
		player->updatePlayer(playerX, playerY, world->getMap());
		player->shootFireball(swarm, dt, playerX, playerY,bigFireBall);
		swarm.checkFireballCollisions(*player);

		// Generate new chunks based on player's position
		
		// Update the camera based on the player's new position
		swarm.update(canvas, *player, dt, playerX, playerY);
		// Draw the map with respect to the camera's position
		world->drawMap(canvas,*player,player->x, player->y);

		// Draw the player with camera offset
		player->drawPLayer(canvas);
		player->drawFireballs(canvas);
		swarm.draw(canvas);
		
		

		frameTime += dt;  // Accumulate time
		frameCount++;     // Increment frame counter

		// If one second has passed, calculate FPS
		if (frameTime >= 1.0f) {
			fps = frameCount;  // Set FPS as the number of frames in the last second
			frameTime = 0.0f;  // Reset time counter
			frameCount = 0;    // Reset frame counter
			  // Output the FPS to the console
		}
		showTimer->fps = fps;
		showTimer->draw(canvas);

		canvas.present();
		totalTime += dt;

		

		if (player->health <= 0) {
			running = false;
			SaveLoad::deleteSave(world->surviveMode);
		}
		if (world->surviveMode&& totalTime>120) {
			running = false;
			SaveLoad::deleteSave(world->surviveMode);
		}
		if (canvas.keyPressed('Q')) {
			SaveLoad::saveGame(*player, *world, showTimer->elapsedTime, swarm);
			running = false;
		}
	}
	

	return 0;
}
