#pragma once

#include <iostream>
#include <vector>
#include <deque> 
#include <algorithm>
#include <chrono>
#include "ClsSnake.h"
#include "ClsFood.h"
#include "ClsBoard.h"

class ClsGameManager {
public:
    enum class GameState { StartMenu, Running, Paused, GameOver, LevelUp, Victory };

private:
    ClsSnake  _Snake;
    ClsFood   _Food;
    ClsBoard  _Board;

    GameState _CurrentState;
    int _Score;
    int _HighScore;
    int _Level;
    unsigned int _GameSpeed;
    int _TargetScoreForNextLevel;
    bool _HardMode;

    std::chrono::steady_clock::time_point _LastUpdateTime;
    EnDirection _LastProcessedDirection;
    bool _InputBufferLocked;

    // Internal method to prepare next level settings and obstacles
    void _SetupLevel() 
    {
        _Board.ClearObstacles();

        // Increase obstacles based on current level
        int obstacleCount = (_Level - 1) * 3;
        if (_Level > 1) 
        {
            // Generate obstacles ensuring they don't overlap with Snake or current Food
            _Board.GenerateRandomObstacles(obstacleCount, _Snake.GetBody(), _Food.GetPosition());
        }

        // Update the target score (Each level requires an additional 100 points)
        _TargetScoreForNextLevel = _Level * 100;

        // Balance game speed based on level and mode (Hard mode is faster)
        _GameSpeed = _HardMode ? std::max<unsigned int>(40, 140 - (_Level * 15))
            : std::max<unsigned int>(60, 160 - (_Level * 10));

        // Re-randomize food position after board obstacles change to avoid spawning under obstacles
        _Food.Reset(_Board.GetRandomEmptyPosition(_Snake.GetBody()));
    }

public:
    ClsGameManager(int width, int height)
        : _Board(width, height),
        _Snake(width / 2, height / 2),
        _Food(width, height),
        _CurrentState(GameState::StartMenu),
        _Score(0),
        _HighScore(0),
        _Level(1),
        _GameSpeed(150),
        _TargetScoreForNextLevel(100),
        _HardMode(false),
        _InputBufferLocked(false) {

        _LastUpdateTime = std::chrono::steady_clock::now();
        _LastProcessedDirection = _Snake.GetDirection();

        // Initial food placement at a valid empty position
        _Food.Reset(_Board.GetRandomEmptyPosition(_Snake.GetBody()));
    }

    // --- Game Flow Control Methods ---

    void StartGame() 
    {
        _CurrentState = GameState::Running;
        _LastUpdateTime = std::chrono::steady_clock::now();
    }

    void TogglePause() 
    {
        if (_CurrentState == GameState::Running) 
        {
            _CurrentState = GameState::Paused;
        }
        else if (_CurrentState == GameState::Paused) 
        {
            _CurrentState = GameState::Running;
            // Reset timer to prevent the snake from "jumping" due to accumulated paused time
            _LastUpdateTime = std::chrono::steady_clock::now();
        }
    }

    // Transitions the game from LevelUp screen back to Running state
    void ContinueNextLevel()
    {
        if (_CurrentState == GameState::LevelUp) 
        {
            _CurrentState = GameState::Running;
            _LastUpdateTime = std::chrono::steady_clock::now();
        }
    }

    void SetHardMode(bool enable) 
    {
        _HardMode = enable;
        // Immediate speed update if mode is changed during gameplay
        _GameSpeed = _HardMode ? std::max<unsigned int>(40, 140 - (_Level * 15))
            : std::max<unsigned int>(60, 160 - (_Level * 10));
    }

    // --- Core Engine Logic ---

    // Checks if enough time has passed to trigger a frame update (Delta Time)
    bool IsTimeForUpdate() 
    {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _LastUpdateTime).count();

        if (duration >= _GameSpeed) 
        {
            _LastUpdateTime = now;
            return true;
        }

        return false;
    }

    // Safely handles player input with anti-180-turn protection
    void HandleInput(EnDirection newDir) {
        if (_CurrentState != GameState::Running || _InputBufferLocked) return;

        // Prevent reversing direction instantly (which causes immediate self-collision)
        if ((newDir == EnDirection::Up && _LastProcessedDirection != EnDirection::Down) ||
            (newDir == EnDirection::Down && _LastProcessedDirection != EnDirection::Up) ||
            (newDir == EnDirection::Left && _LastProcessedDirection != EnDirection::Right) ||
            (newDir == EnDirection::Right && _LastProcessedDirection != EnDirection::Left))
        {

            _Snake.ChangeDirection(newDir);
            _InputBufferLocked = true; // Lock input until the next movement update frame
        }
    }

    // Main update logic called every game tick
    void Update() 
    {
        if (_CurrentState != GameState::Running) return;

        _InputBufferLocked = false; // Unlock input for the new frame
        _LastProcessedDirection = _Snake.GetDirection();
        _Snake.Move();

        // 1. Collision Check (Walls, Self-Body, or Board Obstacles)
        if (_Board.IsWall(_Snake.GetHead()) || _Snake.CheckSelfCollision() || _Board.IsObstacle(_Snake.GetHead())) 
        {
            _CurrentState = GameState::GameOver;
            return;
        }

        // 2. Food Consumption Check
        if (_Food.IsAtPosition(_Snake.GetHead())) 
        {
            _Snake.Eat();
            _Score += _Food.GetScoreValue() * (_HardMode ? 2 : 1);
            if (_Score > _HighScore) _HighScore = _Score;

            // 3. Level Up Check
            if (_Score >= _TargetScoreForNextLevel) 
            {
                _Level++;
                _CurrentState = GameState::LevelUp; // System waits for player to trigger ContinueNextLevel()
                _SetupLevel();
            }
            else 
            {
                // Generate new food only if no level transition occurred
                _Food.Reset(_Board.GetRandomEmptyPosition(_Snake.GetBody()));
            }
        }

        // Update food status (handles TicksToLive for special food types)
        _Food.Update();
    }

    // Full game reset to initial level 1 state
    void Restart() 
    {
        _Score = 0;
        _Level = 1;
        _TargetScoreForNextLevel = 100;
        _GameSpeed = 150;
        _Snake.Reset(_Board.GetWidth() / 2, _Board.GetHeight() / 2);
        _Board.ClearObstacles();
        _Food.Reset(_Board.GetRandomEmptyPosition(_Snake.GetBody()));
        _CurrentState = GameState::Running;
        _LastUpdateTime = std::chrono::steady_clock::now();
    }

    // --- Getters ---
    int GetScore() const { return _Score; }
    int GetHighScore() const { return _HighScore; }
    int GetLevel() const { return _Level; }
    GameState GetState() const { return _CurrentState; }
    const ClsSnake& GetSnake() const { return _Snake; }
    const ClsFood& GetFood() const { return _Food; }
    const ClsBoard& GetBoard() const { return _Board; }
    bool IsHardMode() const { return _HardMode; }
};