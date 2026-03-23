#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <deque>
#include <ctime>
#include "GameTypes.h"

class ClsBoard
{
private:
    int _Width;
    int _Height;
    char _WallChar;
    char _EmptyChar;

    struct PointCompare
    {
        bool operator()(const StrPoint& a, const StrPoint& b) const
        {
            if (a.x != b.x) return a.x < b.x;
            return a.y < b.y;
        }
    };

    std::set<StrPoint, PointCompare> _Obstacles;

public:
    ClsBoard(int width, int height, char wallChar = '#', char emptyChar = ' ')
        : _Width(width), _Height(height), _WallChar(wallChar), _EmptyChar(emptyChar)
    {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
    }

    bool IsWall(StrPoint p) const
    {
        if (p.x <= 0 || p.x >= _Width - 1 || p.y <= 0 || p.y >= _Height - 1)
            return true;
        return IsObstacle(p);
    }

    void AddObstacle(int x, int y)
    {
        _Obstacles.insert({ x, y });
    }

    bool IsInside(int x, int y) const
    {
        return (x > 0 && x < _Width - 1 && y > 0 && y < _Height - 1);
    }

    bool IsObstacle(StrPoint p) const
    {
        return _Obstacles.find(p) != _Obstacles.end();
    }

    StrPoint GetRandomEmptyPosition(const std::deque<StrPoint>& snakeBody) const
    {
        StrPoint p;
        bool occupied;
        int attempts = 0; // Safety counter

        do {
            p.x = (std::rand() % (_Width - 2)) + 1;
            p.y = (std::rand() % (_Height - 2)) + 1;

            bool isSnake = std::find(snakeBody.begin(), snakeBody.end(), p) != snakeBody.end();
            occupied = IsObstacle(p) || isSnake;
            attempts++;

        } while (occupied && attempts < 1000); // Stop after 1000 tries to prevent freezing

        return p;
    }

    // Single optimized function to generate obstacles
    void GenerateRandomObstacles(int count, const std::deque<StrPoint>& snakeBody, StrPoint foodPos)
    {
        _Obstacles.clear();
        for (int i = 0; i < count; i++)
        {
            StrPoint p = GetRandomEmptyPosition(snakeBody);

            // Avoid placing obstacle on the food position
            if (!(p.x == foodPos.x && p.y == foodPos.y))
            {
                _Obstacles.insert(p);
            }
        }
    }

    // Getters
    int GetWidth() const { return _Width; }
    int GetHeight() const { return _Height; }
    char GetWallChar() const { return _WallChar; }
    char GetEmptyChar() const { return _EmptyChar; }
    const std::set<StrPoint, PointCompare>& GetObstacles() const { return _Obstacles; }

    void Resize(int newWidth, int newHeight)
    {
        _Width = newWidth;
        _Height = newHeight;
        _Obstacles.clear();
    }

    void ClearObstacles() { _Obstacles.clear(); }
};