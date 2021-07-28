#pragma once
#include "point.h"
#include <utility>
void product4(float res[4][4], float mat2[4][4], float mat1[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            res[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                res[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

inline float radian(int degree)
{
    return 3.1415 * degree / 180;
}

void transform_point(Point &p, float mat[4][4])
{
    Point temp;
    temp.x = mat[0][0] * p.x + mat[0][1] * p.y + mat[0][2] * p.z + mat[0][3];
    temp.y = mat[1][0] * p.x + mat[1][1] * p.y + mat[1][2] * p.z + mat[1][3];
    temp.z = mat[2][0] * p.x + mat[2][1] * p.y + mat[2][2] * p.z + mat[2][3];
    p = std::move(temp);
}

void scale3d(Point &p, int s)
{
    p = p.operator*(s);
}

void translate3d(Point &p, Point t)
{
    p = p.operator+(t);
}

void rotateX(Point &p, float angle)
{
    angle = radian(angle);
    float temp[4][4] = {
        {1, 0, 0, 0},
        {0, cos(angle), -sin(angle), 0},
        {0, sin(angle), cos(angle), 0},
        {0, 0, 0, 1}};
    transform_point(p, temp);
}

void rotateY(Point &p, float angle)
{
    angle = radian(angle);
    float temp[4][4] = {
        {cos(angle), 0, sin(angle), 0},
        {0, 1, 0, 0},
        {-sin(angle), 0, cos(angle), 0},
        {0, 0, 0, 1}};
    transform_point(p, temp);
}

void rotateZ(Point &p, float angle)
{
    angle = radian(angle);
    float temp[4][4] = {
        {cos(angle), -sin(angle), 0, 0},
        {sin(angle), cos(angle), 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}};
    transform_point(p, temp);
}

void shear(Point &p, float shxy, float shxz, float shyx, float shyz, float shzx, float shzy)
{
    Point temp;
    temp.x = p.x + shxy * p.y + shxz * p.z;
    temp.y = p.y + shyx * p.x + shyz * p.z;
    temp.z = p.z + shzx * p.x + shzy * p.y;
    p = temp;
}

void reflection(Point &p, int axis)
{
    if (axis == 1)
    { //xy plane
        p.z = -p.z;
        }

    if (axis == 2) //xz plane
        p.y = -p.y;

    if (axis == 3) //yz plane
        p.x = -p.x;
}