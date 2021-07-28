#include <iostream>
#include "graphics.h"
#include "3d_transformation.h"
#include "2d_shapes.h"
#include "point.h"
#include <utility>

void update(int value);

void view(Point &inout, Point view)
{
	// view
	// translate + rotate
	inout = inout + -view;
	// rotate
	// u,v,n axes
	// here same as x,y,n
	// float mat[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	float mat[4][4] = {{1 / 1.41, 1 / 1.41, 0, 0},	//  U axis
					   {1 / 1.41, -1 / 1.41, 0, 0}, // V axis
					   {0, 0, 1, 0},				// N axis
					   {0, 0, 0, 1}};
	transform_point(inout, mat);
}

// oblique
void oblique(Point &p, int zvp, float alpha, float phi)
{
	Point temp;
	float l1 = 1.0 / tan(radian(alpha));
	temp.x = int(p.x + l1 * (zvp - p.z) * cos(radian(phi)));
	temp.y = int(p.y + l1 * (zvp - p.z) * sin(radian(phi)));
	temp.z = 1;
	p = std::move(temp);
}

void perspective(Point &p, int zvp, Point prp)
{
	float t1 = (1.0 * prp.z - zvp) / (prp.z - p.z);
	float t2 = (1.0 * zvp - p.z) / (prp.z - p.z);
	Point temp;
	temp.x = p.x * t1 + prp.x * t2;
	temp.y = p.y * t1 + prp.y * t2;
	temp.z = 1;

	// std::cout << "(" << temp.x << "," << temp.y << ")" << std::endl;

	p = std::move(temp);
}

void plotcube(Point cube[8])
{
	Bresenham(cube[0].x, cube[0].y, cube[1].x, cube[1].y);
	Bresenham(cube[0].x, cube[0].y, cube[2].x, cube[2].y);
	Bresenham(cube[0].x, cube[0].y, cube[4].x, cube[4].y);
	Bresenham(cube[1].x, cube[1].y, cube[3].x, cube[3].y);
	Bresenham(cube[1].x, cube[1].y, cube[5].x, cube[5].y);
	Bresenham(cube[2].x, cube[2].y, cube[3].x, cube[3].y);
	Bresenham(cube[5].x, cube[5].y, cube[4].x, cube[4].y);
	Bresenham(cube[2].x, cube[2].y, cube[6].x, cube[6].y);
	Bresenham(cube[7].x, cube[7].y, cube[6].x, cube[6].y);
	Bresenham(cube[4].x, cube[4].y, cube[6].x, cube[6].y);
	Bresenham(cube[7].x, cube[7].y, cube[3].x, cube[3].y);
	Bresenham(cube[7].x, cube[7].y, cube[5].x, cube[5].y);
}

Point cube[8];

int main(int argc, char **argv)
{
	init(&argc, argv);

	// draw the 2d axis
	for (int i = -WIDTH / 2; i < WIDTH / 2; i++)
	{
		putpixel(0, i, 3);
		putpixel(i, 0, 3);
	}

	// Point p = {0, 0, 0};
	// perspective(p, 400, {100, 100, 1000});
	// std::cout << p.x << " " << p.y << " " << p.z << std::endl;

	update(0);
	glutMainLoop();
	return 0;
}

void cleargrid()
{
	for (GLint x = 0; x < WIDTH * HEIGHT; ++x)
	{
		grid[x] = 0;
	}
}

void floodFill(int x, int y, const Color &color)
{
	auto value = getpixel(x, y);
	if (value.r == 1 && value.g == 1 && value.b == 1)
	{
		putpixel(x, y, color);
		floodFill(x + 1, y, color);
		floodFill(x - 1, y, color);
		floodFill(x, y + 1, color);
		floodFill(x, y - 1, color);
	}
}

int theta = 0;
// PUT THINGS THAT NEED UPDATING EVERY FRAME HERE
void update(int value)
{
	//Do update stuff here
	//noting for now
	// main code here
	theta += 0;
	theta %= 360;

	cleargrid();

	// main code here
	cube[0] = {0, 0, 0};
	cube[1] = {0, 0, 1};
	cube[2] = {0, 1, 0};
	cube[3] = {0, 1, 1};
	cube[4] = {1, 0, 0};
	cube[5] = {1, 0, 1};
	cube[6] = {1, 1, 0};
	cube[7] = {1, 1, 1};

	for (int i = 0; i < 8; i++)
	{
		scale3d(cube[i], 200);
		translate3d(cube[i], {-100, -100, -100});
		reflection(cube[i], 1);
		view(cube[i], {0, 0, 200});
		// oblique(cube[i], 10, 45, 45);
		perspective(cube[i], 200, {0, 0, 300});
	}
	plotcube(cube);
	Color ff_color(1, 0, 0);
	floodFill(-200, 0, ff_color);
	putpixel(0, 0, {1, 0, 0});
	// mark this window needs to be rerendered
	// hence call display() callback
	glutPostRedisplay();
	// after certain time call update again
	glutTimerFunc(1000 / FPS, update, 0);
}