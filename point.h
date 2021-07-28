#pragma once
#include <math.h>

struct Point
{
    int x, y, z;
    Point operator+(Point const &obj)
    {
        return {x + obj.x, y + obj.y, z + obj.y};
    }
    Point operator-()
    {
        return {-x, -y, -z};
    }
    Point operator*(float a)
    {
        return {int(a * x), int(a * y), int(a * z)};
    }
};

Point &normalize(Point &p)
{
    float mag = p.x * p.x + p.y * p.y + p.z * p.z;
    mag = pow(mag, 0.5);
    p.x /= mag;
    p.y /= mag;
    p.z /= mag;
    return p;
}