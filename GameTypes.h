#pragma once

enum class EnDirection { Up = 0, Down = 1, Left = 2, Right = 3 };

struct StrPoint 
{
    int x, y;

    bool operator==(const StrPoint& other) const
    {
        return x == other.x && y == other.y;
    }
};

