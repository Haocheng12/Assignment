#include <fstream>
#include <iostream>
#include <string>
#include "Object.h"
#include "World.h"
#include <iomanip> // For std::fixed and std::setprecision


class SaveLoad {
public:
    // Save the game state to a file
    static void saveGame(const Player& player, const World& world, float totalTime) {
        std::ofstream saveFile("savegame.txt");
        if (saveFile.is_open()) {
            saveFile << player.x << " " << player.y << " " << player.health << " " << player.score <<" "<< player.attackSpeed<< " " <<player.speed<<"\n";
            saveFile << totalTime << "\n";
            saveFile << world.surviveMode << "\n"; // Example for the world state
            saveFile.close();
            std::cout << "Game saved successfully.\n";
        }
        else {
            std::cerr << "Failed to save the game.\n";
        }
      
    }

    // Load the game state from a file
    static bool loadGame(Player& player, World& world,ShowTimer& showTimer) {
        std::ifstream saveFile("savegame.txt");
        if (saveFile.is_open()) {
            float x, y, totalTime;
            int health, score, attackSpeed,speed;
            bool surviveMode;
            saveFile >> x >> y >> health >> score>>attackSpeed>>speed>> totalTime;
            saveFile >> totalTime;
            if (saveFile.fail()) {
                std::cerr << "Error reading totalTime." << std::endl;
               
            }
            saveFile >> surviveMode;

            // Load data into player and world
            player.x = x;
            player.y = y;
            player.health = health;
            player.score = score;
            player.attackSpeed = attackSpeed;
            player.speed = speed;
            cout << "TOTAL"<<totalTime<<endl;
            showTimer.elapsedTime = totalTime;
            world.surviveMode = surviveMode;

            saveFile.close();
            std::cout << "Game loaded successfully.\n";
            return true; // Game was loaded successfully
        }
        else {
            std::cerr << "No save file found. Starting a new game.\n";
            return false; // No save file, start a new game
        }
    }

    // Delete the save file (when the game completes)
    static void deleteSave() {
        if (std::remove("savegame.txt") == 0) {
            std::cout << "Save file deleted.\n";
        }
        else {
            std::cerr << "Failed to delete save file.\n";
        }
    }
};
