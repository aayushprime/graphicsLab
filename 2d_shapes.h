// Line drawing algorithm
#include <math.h>

void putpixel(int x, int y, int color = 1);

void Bresenham(int x1, int yy, int x2, int y2, int color = 1)
{
    int dx, dy;
    int steps, k;

    dx = abs(x2 - x1);
    dy = abs(y2 - yy);

    int lx, ly;
    if (x2 > x1)
        lx = 1;
    else
        lx = -1;

    if (y2 > yy)
        ly = 1;
    else
        ly = -1;

    int x = x1, y = yy;
    // slope < 1
    if (dx > dy)
    {
        int p = 2 * dy - dx;
        for (int k = 0; k <= dx; k++)
        {
            putpixel(x, y, color);
            if (p < 0)
            {
                x += lx;
                p += 2 * dy;
            }
            else
            {
                x += lx;
                y += ly;
                p += 2 * dy - 2 * dx;
            }
        }
    }
    else
    { //slope > 1
        int p = 2 * dx - dy;
        for (int k = 0; k <= dy; k++)
        {
            putpixel(x, y, color);
            if (p < 0)
            {
                y += ly;
                p += 2 * dx;
            }
            else
            {
                x += lx;
                y += ly;
                p += 2 * dx - 2 * dy;
            }
        }
    }
    // putpixel(x, y, color);
}

void draw_triangle(float triangle[3][3], int color = 1)
{
    Bresenham(triangle[0][0], triangle[1][0], triangle[0][1], triangle[1][1], color);
    Bresenham(triangle[0][1], triangle[1][1], triangle[0][2], triangle[1][2], color);
    Bresenham(triangle[0][2], triangle[1][2], triangle[0][0], triangle[1][0], color);
}
