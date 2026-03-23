#pragma once

#include <vector>
#include <algorithm> // Provides a collection of ready-to-use functions for searching, sorting, and transforming data.
#include <deque> // A dynamic array that allows fast insertion and deletion at both the beginning and the end.
#include "GameTypes.h"

using namespace std;

class ClsSnake
{
private:
    deque<StrPoint> _body;
    EnDirection _currentDir;
    EnDirection _lastProcessedDir;
    bool _shouldGrow;

public:
    ClsSnake(int startX, int startY)
    {
        Reset(startX, startY);
    }

    void Reset(int startX, int startY)
    {
        _body.clear();

        // Start with head and one tail segment
        _body.push_front({ startX - 1, startY }); // head
        _body.push_front({ startX, startY }); // Tail

        _currentDir = EnDirection::Right;
        _lastProcessedDir = EnDirection::Right;
        _shouldGrow = false;
    }

    void ChangeDirection(EnDirection newDir)
    {
        if (newDir == EnDirection::Up && _lastProcessedDir == EnDirection::Down) return;
        if (newDir == EnDirection::Down && _lastProcessedDir == EnDirection::Up) return;
        if (newDir == EnDirection::Left && _lastProcessedDir == EnDirection::Right) return;
        if (newDir == EnDirection::Right && _lastProcessedDir == EnDirection::Left) return;

        _currentDir = newDir;
    }

    void Move()
    {
        _lastProcessedDir = _currentDir;
        StrPoint newHead = _body.front();

        switch (_currentDir)
        {
        case EnDirection::Up:    
            newHead.y--; 
            break;
        case EnDirection::Down:  
            newHead.y++; 
            break;
        case EnDirection::Left:  
            newHead.x--; 
            break;
        case EnDirection::Right: 
            newHead.x++; 
            break;
        }

        _body.push_front(newHead);

        if (!_shouldGrow) _body.pop_back();
        else _shouldGrow = false;
    }

    void Eat() { _shouldGrow = true; }

    bool CheckSelfCollision() const
    {
        if (_body.size() < 5) return false;
        return find(_body.begin() + 1, _body.end(), _body.front()) != _body.end();
    }

    bool IsAtPosition(int x, int y) const
    {
        return find(_body.begin(), _body.end(), StrPoint{ x, y }) != _body.end();
    }

    bool CheckWallCollision(int maxWidth, int maxHeight) const
    {
        StrPoint head = GetHead();
        return (head.x < 0 || head.x >= maxWidth || head.y < 0 || head.y >= maxHeight);
    }

    // Getters
    StrPoint GetHead() const { return _body.front(); }
    EnDirection GetDirection() const { return _currentDir; }
    const deque<StrPoint>& GetBody() const { return _body; }
    int GetLength() const { return (int)_body.size(); }
};