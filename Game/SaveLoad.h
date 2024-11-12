#include <fstream>
#include <iostream>
#include <string>
#include "Object.h"
#include "World.h"
#include <iomanip> // For std::fixed and std::setprecision

using namespace std;

class SaveLoad {
public:
    // Save the game state to a file
    static void saveGame(const Player& player, const World& world, float totalTime, const Swarm& swarm) {
        string s;
        if (world.surviveMode) {
            s=("SurviveSave.txt");
            
        }
        else {
            s=("EndlessSave.txt");
            world.saveMap();
        }
        ofstream saveFile(s);
            if (saveFile.is_open()) {
                // Save player data
                saveFile << player.x << " " << player.y << " "
                    << player.health << " " << player.score << " "
                    << player.attackSpeed << " " << player.speed << "\n";

                // Save game time and world state
                saveFile << totalTime << "\n";
             
                swarm.save(saveFile);
                saveFile.close();
                cout << "Game saved successfully.\n";
            }
            else {
                cerr << "Failed to save the game.\n";
                deleteSave(world.surviveMode);
            }
        
        
    }


    // Load the game state from a file
    static bool loadGame(Player& player, World& world, ShowTimer& showTimer, Swarm& swarm) {
        string s;
        if (world.surviveMode) {
            s = ("SurviveSave.txt");
           
        }
        else {
            s = ("EndlessSave.txt");

        }
        ifstream loadFile(s);
        if (loadFile.is_open()) {
            float x, y, totalTime, attackSpeed, speed;
            int health, score;
          

            // Load Player data
            loadFile >> x >> y >> health >> score >> attackSpeed >> speed;
            player.x = x;
            player.y = y;
            player.health = health;
            player.score = score;
            player.attackSpeed = attackSpeed;
            player.speed = speed;

            // Load Total Time and World data
            loadFile >> totalTime;
            showTimer.elapsedTime = totalTime;
          
        

            // Load the Swarm data
            swarm.load(loadFile);

            loadFile.close();
            std::cout << "Game loaded successfully.\n";
            return true;
        }
        else {
            std::cerr << "No save file found. Starting a new game.\n";
            return false;
        }
    }



    // Delete the save file (when the game completes)
    static void deleteSave(bool surviveMode) {
        if (surviveMode) {
            remove("SurviveSave.txt");
        }
        else {
            remove("EndlessSave.txt");
        }
        
    }
};
