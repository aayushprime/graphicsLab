#pragma once
#include <math.h>

struct Point
{
    int x, y, z;
    Point() {}
    Point(int x, int y, int z) : x(x), y(y), z(z)
    {
    }
    Point operator+(Point const &obj)
    {
        return {x + obj.x, y + obj.y, z + obj.z};
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
    double p1 = 1.0f * p.x * p.x;
    double p2 = 1.0f * p.y * p.y;
    double p3 = 1.0f * p.z * p.z;
    // double mag = p.x * p.x + p.y * p.y + p.z * p.z;
    double mag = p1 + p2 + p3;
    mag = sqrt(mag);
    p.x /= mag;
    p.y /= mag;
    p.z /= mag;
    return p;
}