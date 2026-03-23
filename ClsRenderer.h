#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream> // ··≈÷«›… «·ÃœÌœ…
#include "ClsGameManager.h"

class ClsRenderer 
{
private:
    const std::string RESET = "\033[0m";
    const std::string GREEN = "\033[32m";
    const std::string RED = "\033[31m";
    const std::string BLUE = "\033[34m";
    const std::string YELLOW = "\033[33m";
    const std::string CYAN = "\033[36m";
    const std::string MAGENTA = "\033[35m";

    void _ResetCursor() {
        std::cout << "\033[H";
    }

public:
    void InitializeConsole() {
        std::cout << "\033[?25l";
        ClearScreen();
    }

    void ShutdownConsole() {
        std::cout << "\033[?25h" << RESET;
    }

    void ClearScreen() {
        std::cout << "\033[2J\033[3J\033[H";
    }

    void Render(const ClsGameManager& game) {
        _ResetCursor();

        auto& board = game.GetBoard();
        auto& snake = game.GetSnake();
        auto& food = game.GetFood();
        auto  state = game.GetState();

        // «” Œœ«„ stringstream · Ã„Ì⁄ «·—”„ ﬂ·Â Êÿ»«⁄ Â „—… Ê«Õœ… (√”—⁄ »ﬂ Ì—)
        std::stringstream ss;

        // 1. Header
        ss << YELLOW << " [ Score: " << game.GetScore()
            << " ] | [ Level: " << game.GetLevel()
            << " ] | [ High: " << game.GetHighScore() << " ]" << RESET << "        \n";

        // 2. Render Board
        for (int y = 0; y < board.GetHeight(); ++y) {
            for (int x = 0; x < board.GetWidth(); ++x) {
                StrPoint currentPos = { x, y };

                if (board.IsWall(currentPos)) {
                    ss << BLUE << board.GetWallChar() << RESET;
                }
                else if (snake.GetHead().x == x && snake.GetHead().y == y) {
                    ss << GREEN << "@" << RESET;
                }
                else if (snake.IsAtPosition(x, y)) {
                    ss << GREEN << "o" << RESET;
                }
                else if (food.IsAtPosition(currentPos)) {
                    if (food.GetType() == FoodType::Bonus)
                        ss << MAGENTA << food.GetFoodChar() << RESET;
                    else if (food.GetType() == FoodType::Poison)
                        ss << RED << food.GetFoodChar() << RESET;
                    else
                        ss << YELLOW << food.GetFoodChar() << RESET;
                }
                else if (board.IsObstacle(currentPos)) {
                    ss << CYAN << "!" << RESET;
                }
                else {
                    ss << board.GetEmptyChar();
                }
            }
            ss << "\n";
        }

        // 3. Overlays
        ss << "\n";
        switch (state) {
        case ClsGameManager::GameState::StartMenu:
            ss << GREEN << "   === SNAKE GAME CLI ===   " << RESET << "\n"
                << YELLOW << "   Use Arrows to Move       " << RESET << "\n"
                << GREEN << "   Press ENTER to Start     " << RESET << "\n";
            break;
        case ClsGameManager::GameState::Paused:
            ss << YELLOW << "   || GAME PAUSED ||        " << RESET << "\n"
                << "   Press 'P' to Resume      \n";
            break;
        case ClsGameManager::GameState::GameOver:
            ss << RED << "   !!! GAME OVER !!!        " << RESET << "\n"
                << "   Final Score: " << game.GetScore() << "\n"
                << "   Press 'R' to Restart     \n";
            break;
        case ClsGameManager::GameState::LevelUp:
            ss << CYAN << "   *** LEVEL COMPLETE! *** " << RESET << "\n"
                << "   Next: Level " << game.GetLevel() << "\n"
                << "   Press SPACE to Begin     \n";
            break;
        case ClsGameManager::GameState::Victory:
            ss << MAGENTA << "   ?? CHAMPION! ??          " << RESET << "\n"
                << "   You cleared all levels!  \n"
                << "   Press 'Esc' to exit.     \n";
            break;
        }

        // ÿ»«⁄… ﬂ· ‘Ì¡ „—… Ê«Õœ…
        std::cout << ss.str();
    }
};