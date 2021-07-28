#pragma once
#include <math.h>
#include <iostream>

// Function to round to float values
int Round(float a)
{
    int b = a + 0.5;
    return b;
}

// convert degree to radian
inline float radian(int degree)
{
    return 3.1415 * degree / 180;
}

void product(float mat1[3][3], float mat2[3][3], float mat3[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            mat3[i][j] = 0;
            for (int k = 0; k < 3; k++)
            {
                mat3[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

void translate(int tx, int ty, float res[3][3], float triangle[3][3])
{
    float trans_matrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    trans_matrix[0][2] = tx;
    trans_matrix[1][2] = ty;

    product(trans_matrix, triangle, res);
}

void rotation(int rx, int ry, float theta, float res[3][3], float triangle[3][3])
{

    translate(-rx, -ry, res, triangle);

    float rotation_matrix[3][3] = {{cos(radian(theta)), -sin(radian(theta)), 0}, {sin(radian(theta)), cos(radian(theta)), 0}, {0, 0, 1}};
    float temp[3][3] = {0};
    product(rotation_matrix, res, temp);

    translate(rx, ry, res, temp);
}

void scaling(int rx, int ry, int sx, int sy, float res[3][3], float triangle[3][3])
{
    translate(-rx, -ry, res, triangle);

    float scale_matrix[3][3] = {{sx, 0, 0}, {0, sy, 0}, {0, 0, 1}};
    float temp[3][3] = {0};
    product(scale_matrix, res, temp);

    translate(+rx, +ry, res, temp);
}

// reflection about axis
// 1: x axis, 2: y axis, 3: x=y, 4: y = -x
void reflection(int axis, float res[3][3], float triangle[3][3])
{
    float transformation_matrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    if (axis == 1)
        transformation_matrix[1][1] = -1;
    else if (axis == 2)
        transformation_matrix[0][0] = -1;
    else if (axis == 3)
    {
        transformation_matrix[0][0] = 0;
        transformation_matrix[0][1] = +1;
        transformation_matrix[1][0] = +1;
        transformation_matrix[1][1] = 0;
    }
    else if (axis == 4)
    {
        transformation_matrix[0][0] = 0;
        transformation_matrix[0][1] = -1;
        transformation_matrix[1][0] = -1;
        transformation_matrix[1][1] = 0;
    }
    else
    {
        std::cout << "invalid reflection axis" << std::endl;
        return;
    }

    product(transformation_matrix, triangle, res);
}

void shearing(int axis, float sh, float res[3][3], float triangle[3][3])
{
    float shear_matrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    if (axis == 1)
        // y-axis
        shear_matrix[1][0] = sh;
    else if (axis == 2)
        // x-axis
        shear_matrix[0][1] = sh;
    else
        std::cout << "invalid shearing option" << std::endl;

    product(shear_matrix, triangle, res);
}

void shearing_ref(int axis, float ref, float sh, float res[3][3], float triangle[3][3])
{
    float shear_matrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    if (axis == 1)
    {
        // y-axis
        shear_matrix[1][0] = sh;
        shear_matrix[1][2] = -sh * ref;
    }
    else if (axis == 2)
    {
        // x-axis
        shear_matrix[0][1] = sh;
        shear_matrix[0][2] = -sh * ref;
    }

    else
        std::cout << "invalid shearing option" << std::endl;

    product(shear_matrix, triangle, res);
}

void scale_about_arbitrary_axis(float theta, float sx, float sy, float res[3][3], float triangle[3][3])
{
    float temp[3][3];
    // rotate - theta
    rotation(0, 0, -theta, res, triangle);
    // scale
    scaling(0, 0, sx, sy, temp, res);
    // rotate + theta
    rotation(0, 0, +theta, res, temp);
}
