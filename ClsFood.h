#pragma once

#include <iostream>
#include <deque>
#include <algorithm>
#include <ctime>
#include "GameTypes.h"

enum class FoodType { Normal, Bonus, Poison };

class ClsFood 
{
private:
    StrPoint _Position;
    int _MaxX;
    int _MaxY;
    int _ScoreValue;
    FoodType _Type;
    int _TicksToLive;

public:
    ClsFood(int maxX, int maxY)
        : _MaxX(maxX), _MaxY(maxY), _TicksToLive(-1), _ScoreValue(10), _Type(FoodType::Normal) 
    {
        _Position = { 0, 0 };
    }

    void Reset(StrPoint newPos, FoodType type = FoodType::Normal, int duration = -1) 
    {
        _Position = newPos;
        _Type = type;
        _TicksToLive = duration;

        switch (_Type) 
        {
            case FoodType::Bonus:  
                _ScoreValue = 50;  
                break;
            case FoodType::Poison: 
                _ScoreValue = -20; 
                break;
            default:               
                _ScoreValue = 10;  
                break;
        }
    }

    void Update() 
    {
        if (_TicksToLive > 0) _TicksToLive--;
    }

    bool IsExpired() const { return (_TicksToLive == 0); }

    bool IsActive() const { return (_TicksToLive != 0); }

    bool IsPermanent() const { return (_TicksToLive == -1); }

    void Deactivate() { _TicksToLive = 0; }

    char GetFoodChar() const 
    {
        switch (_Type) 
        {
            case FoodType::Bonus:  
                return '$';
            case FoodType::Poison: 
                return 'X';
            default:               
                return '@';
        }
    }

    // Setters & Getters
    void SetPosition(StrPoint p) { _Position = p; }
    bool IsAtPosition(StrPoint p) const { return _Position.x == p.x && _Position.y == p.y; }
    StrPoint GetPosition() const { return _Position; }
    int GetScoreValue() const { return _ScoreValue; }
    FoodType GetType() const { return _Type; }
};