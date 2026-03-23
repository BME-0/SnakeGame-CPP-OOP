#pragma once

#include <conio.h>    // For keyboard input handling (_kbhit, _getch)
#include <windows.h>  // For timing and control (Sleep)
#include "ClsGameManager.h"
#include "ClsRenderer.h"

class ClsGameLauncher 
{
private:
    ClsGameManager _Game;      // Core game logic controller
    ClsRenderer _Renderer;     // Visual representation handler
    bool _IsRunning;           // Flag to control the main game loop

    // Monitors and processes user keyboard input
    void _ProcessInput() 
    {
        if (_kbhit()) 
        {
            int key = _getch(); // Using int to handle extended key codes

            // Handle special keys like Arrow Keys (prefixed by 0 or 224)
            if (key == 0 || key == 224) 
            {
                key = _getch(); // Fetch the actual scan code for the arrow

                switch (key) 
                {
                    case 72: 
                        _Game.HandleInput(EnDirection::Up);    
                        break; // Up Arrow
                    case 80: 
                        _Game.HandleInput(EnDirection::Down);  
                        break; // Down Arrow
                    case 75: 
                        _Game.HandleInput(EnDirection::Left);  
                        break; // Left Arrow
                    case 77: 
                        _Game.HandleInput(EnDirection::Right); 
                        break; // Right Arrow
                }
            }
            else {
                // Handle standard alphanumeric and control keys
                switch (key) 
                {
                    // Support both WASD and Arrow controls
                case 'w': case 'W': _Game.HandleInput(EnDirection::Up);    break;
                case 's': case 'S': _Game.HandleInput(EnDirection::Down);  break;
                case 'a': case 'A': _Game.HandleInput(EnDirection::Left);  break;
                case 'd': case 'D': _Game.HandleInput(EnDirection::Right); break;

                case 13: // Enter Key: Starts the game from the menu
                    if (_Game.GetState() == ClsGameManager::GameState::StartMenu)
                        _Game.StartGame();
                    break;

                case 'p': case 'P': // P Key: Toggles pause/resume
                    _Game.TogglePause();
                    break;

                case 'r': case 'R': // R Key: Restarts the game after Game Over
                    if (_Game.GetState() == ClsGameManager::GameState::GameOver)
                        _Game.Restart();
                    break;

                case 32: // Space Bar: Continues to the next level
                    if (_Game.GetState() == ClsGameManager::GameState::LevelUp)
                        _Game.ContinueNextLevel();
                    break;

                case 27: // Esc Key: Gracefully exits the application
                    _IsRunning = false;
                    break;
                }
            }
        }
    }

public:
    // Initializes the game with specified board dimensions
    ClsGameLauncher(int width = 40, int height = 20)
        : _Game(width, height), _IsRunning(true) 
    {
    }

    // Starts the main engine: Handles input, logic updates, and rendering
    void Run() 
    {
        _Renderer.InitializeConsole();

        // The Main Game Loop
        while (_IsRunning) 
        {
            _ProcessInput();

            // Update game logic only when the required time interval has passed
            if (_Game.IsTimeForUpdate()) 
            {
                _Game.Update();
            }

            // Draw the current game state to the screen
            _Renderer.Render(_Game);

            // Performance balance: Prevents 100% CPU usage
            Sleep(10);
        }

        // Cleanup: Restores console settings before exiting
        _Renderer.ShutdownConsole();
        std::cout << "\n   Game Exited. Goodbye Beso!\n";
    }
};