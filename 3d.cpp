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
	float mat[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	// float mat[4][4] = {{1 / 1.41, 1 / 1.41, 0, 0},	//  U axis
	// 				   {1 / 1.41, -1 / 1.41, 0, 0}, // V axis
	// 				   {0, 0, 1, 0},				// N axis
	// 				   {0, 0, 0, 1}};
	transform_point(inout, mat);
}

void view(Point &inout, Point eye, Point lookingAt)
{
	// view
	// translate + rotate
	Point view = eye + -lookingAt;
	inout = inout + -view;
	// rotate
	// u,v,n axes
	// here same as x,y,n
	float mat[4][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
	// float mat[4][4] = {{1 / 1.41, 1 / 1.41, 0, 0},	//  U axis
	// 				   {1 / 1.41, -1 / 1.41, 0, 0}, // V axis
	// 				   {0, 0, 1, 0},				// N axis
	// 				   {0, 0, 0, 1}};
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

vec3 cross(const vec3 v, const vec3 &other)
{
	vec3 temp;
	temp.r = v.g * other.b - other.g * v.b;
	temp.g = -v.r * other.b + v.b * other.r;
	temp.b = v.r * other.g - other.r * v.g;
	return std::move(temp);
}

float dot(vec3 v1, vec3 v2)
{
	return v1.r * v2.r + v1.g * v2.g + v1.b * v2.b;
}

float intensity = 10;

vec3 multiply(float f, vec3 v)
{
	return {f * v.r, f * v.g, f * v.b};
}

vec3 sub(vec3 v1, vec3 v2)
{
	return {v1.r - v2.r, v1.g - v2.g, v1.b - v2.b};
}

void plotCubeUsingFaces(Point cube[8], int faces[6][4], vec3 normals[6], Point averagePoint[6], vec3 eyeVector[6], vec3 lightVector[6])
{
	for (size_t i = 0; i < 6; i++)
	{
		// back face detection
		if (normals[i].b <= 0)
			continue;

		Color color(1, 0, 1);
		float kd = 0.1f, ks = 0.1f, ka = 0.01f;

		float ambient = ka;

		normalize(lightVector[i]);
		normalize(normals[i]);
		float diffuse = kd * std::max(dot(lightVector[i], normals[i]), 0.f);

		// r = 2 (N.L) N - L
		//  ks * I (V.R)^ns
		float ns = 3;
		float temp = 2 * dot(normals[i], lightVector[i]);
		vec3 R = sub(multiply(temp, normals[i]), lightVector[i]);
		normalize(eyeVector[i]);
		float specular = ks * std::max(pow(dot(eyeVector[i], R), ns), 0.f);

		float total = intensity * (ambient + diffuse + specular);

		color.r *= total;
		color.g *= total;
		color.b *= total;

		// For each face
		// faces[i]
		Bresenham(cube[faces[i][0]].x, cube[faces[i][0]].y, cube[faces[i][1]].x, cube[faces[i][1]].y);
		Bresenham(cube[faces[i][1]].x, cube[faces[i][1]].y, cube[faces[i][2]].x, cube[faces[i][2]].y);
		Bresenham(cube[faces[i][2]].x, cube[faces[i][2]].y, cube[faces[i][3]].x, cube[faces[i][3]].y);
		Bresenham(cube[faces[i][3]].x, cube[faces[i][3]].y, cube[faces[i][0]].x, cube[faces[i][0]].y);
		floodFill(averagePoint[i].x, averagePoint[i].y, color);
	}
}

int theta = 0;
// PUT THINGS THAT NEED UPDATING EVERY FRAME HERE
void update(int value)
{
	//Do update stuff here
	//noting for now
	// main code here
	theta += 1;
	theta %= 360;

	cleargrid();

	Point light(-200, -200, -200);
	Point eye(0, 0, 200);
	Point lookingAt(0, 0, 100);

	// main code here
	cube[0] = {0, 0, 0};
	cube[1] = {0, 0, 1};
	cube[2] = {0, 1, 0};
	cube[3] = {0, 1, 1};
	cube[4] = {1, 0, 0};
	cube[5] = {1, 0, 1};
	cube[6] = {1, 1, 0};
	cube[7] = {1, 1, 1};

	int faces[6][4] = {{1, 3, 2, 0}, {5, 1, 0, 4}, {0, 2, 6, 4}, {1, 5, 7, 3}, {5, 4, 6, 7}, {2, 3, 7, 6}};

	for (int i = 0; i < 8; i++)
	{
		scale3d(cube[i], 200);
		translate3d(cube[i], {-100, -100, -100});
		rotateX(cube[i], theta);
		// reflection(cube[i], 1);
		// view(cube[i], {0, 0, 200});
		Point eye(0, 0, 200);
		Point lookingAt(0, 0, 100);
		view(cube[i], eye, lookingAt);
		// oblique(cube[i], 10, 45, 45);
	}

	vec3 normals[6];
	vec3 eyeVector[6];
	vec3 lightVector[6];
	Point averagePoint[6];

	for (int i = 0; i < 6; i++)
	{
		// normal calculation
		auto vector1 = cube[faces[i][1]] + -cube[faces[i][0]];
		auto vector2 = cube[faces[i][2]] + -cube[faces[i][1]];
		vec3 vec1 = {vector1.x, vector1.y, vector1.z};
		vec3 vec2 = {vector2.x, vector2.y, vector2.z};
		normals[i] = cross(vec1, vec2);
		normalize(normals[i]);

		averagePoint[i].x = cube[faces[i][0]].x + cube[faces[i][1]].x + cube[faces[i][2]].x + cube[faces[i][3]].x;
		averagePoint[i].y = cube[faces[i][0]].y + cube[faces[i][1]].y + cube[faces[i][2]].y + cube[faces[i][3]].y;
		averagePoint[i].z = cube[faces[i][0]].z + cube[faces[i][1]].z + cube[faces[i][2]].z + cube[faces[i][3]].z;
		averagePoint[i].x /= 4;
		averagePoint[i].y /= 4;
		averagePoint[i].z /= 4;

		eyeVector[i] = {averagePoint[i].x - eye.x, averagePoint[i].y - eye.y, averagePoint[i].z - eye.z};
		lightVector[i] = {averagePoint[i].x - light.x, averagePoint[i].y - light.y, averagePoint[i].z - light.z};
	}

	// view transforms !!
	for (int i = 0; i < 8; i++)
	{
		view(cube[i], eye, lookingAt);
	}
	for (size_t i = 0; i < 6; i++)
	{
		view(averagePoint[i], eye, lookingAt);
	}

	// perspective transforms!!
	int zvp = 200;
	Point prp(0, 0, 300);
	for (int i = 0; i < 8; i++)
	{
		perspective(cube[i], zvp, prp);
	}
	for (size_t i = 0; i < 6; i++)
	{
		perspective(averagePoint[i], zvp, prp);
	}

	// plotcube(cube);
	plotCubeUsingFaces(cube, faces, normals, averagePoint, eyeVector, lightVector);

	// Color ff_color(1, 0, 0);
	// floodFill(-200, 0, ff_color);

	// mark this window needs to be rerendered
	// hence call display() callback
	glutPostRedisplay();
	// after certain time call update again
	glutTimerFunc(1000 / FPS, update, 0);
}