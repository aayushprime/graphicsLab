#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#define WHITE 1.0, 1.0, 1.0
#define BLACK 0.0, 0.0, 0.0
#define RED 1.0, 0.0, 0.0
#define GREEN 0.0, 1.0, 0.0
#define BLUE 0.0, 0.0, 1.0

GLint FPS = 60;
GLint window_width = 1200;
GLint window_height = 1200;
GLfloat LEFT = 0.0;
GLfloat RIGHT = 1.0;
GLfloat BOTTOM = 0.0;
GLfloat TOP = 1.0;
GLint WIDTH = 600; //600
GLint HEIGHT = 600;

struct Color
{
    float r, g, b;
    Color() : r(1), g(1), b(1) {}
    Color(int i)
    {
        if (i == 1)
        {
            r = 0;
            g = 0;
            b = 0;
        }
        else if (i == 2)
        {
            r = 1;
            g = 0;
            b = 0;
        }
        else if (i == 3)
        {
            r = 0;
            g = 1;
            b = 0;
        }
        else
        {
            r = 1;
            g = 1;
            b = 1;
        }
    }
    Color(float r, float g, float b) : r(r), g(g), b(b) {}
};

// int *grid;
Color *grid;

using namespace std;
// GRID FORMING CODE
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    GLfloat xSize = (RIGHT - LEFT) / WIDTH;
    GLfloat ySize = (TOP - BOTTOM) / HEIGHT;

    glBegin(GL_QUADS);
    for (GLint x = 0; x < WIDTH; ++x)
    {
        for (GLint y = 0; y < HEIGHT; ++y)
        {
            auto val = grid[x + y * WIDTH];
            glColor3f(val.r, val.b, val.g);
            // if (val == 1)
            //     glColor3f(BLACK);
            // else if (val == 2)
            //     glColor3f(RED);
            // else if (val == 3)
            //     glColor3f(GREEN);
            // else if (val == 4)
            //     glColor3f(BLUE);
            // else
            //     glColor3f(WHITE);
            glVertex2f(x * xSize + LEFT, y * ySize + BOTTOM);
            glVertex2f((x + 1) * xSize + LEFT, y * ySize + BOTTOM);
            glVertex2f((x + 1) * xSize + LEFT, (y + 1) * ySize + BOTTOM);
            glVertex2f(x * xSize + LEFT, (y + 1) * ySize + BOTTOM);
        }
    }
    glEnd();

    glFlush();
    glutSwapBuffers();
}
// 	LITEN FOR WINDOW SIZE CHANGE
void reshape(int w, int h)
{
    window_width = w;
    window_height = h;

    glViewport(0, 0, window_width, window_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(LEFT, RIGHT, BOTTOM, TOP);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutPostRedisplay();
}

void putpixel(int x, int y, int color)
{
    //offset the origin to center
    x = x + WIDTH / 2;
    y = y + HEIGHT / 2;
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        grid[x + y * WIDTH] = color;
}

void putpixel(int x, int y, Color color)
{
    //offset the origin to center
    x = x + WIDTH / 2;
    y = y + HEIGHT / 2;
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        grid[x + y * WIDTH] = color;
}

Color getpixel(int x, int y)
{
    //offset the origin to center
    x = x + WIDTH / 2;
    y = y + HEIGHT / 2;
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        return grid[x + y * WIDTH];
    else
        return {-1, -1, -1};
}

void init(int *argc, char **argv)
{
    glutInit(argc, argv);

    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("3D");
    glClearColor(1, 1, 1, 1);

    glutReshapeFunc(reshape);
    // set redraw function as display
    glutDisplayFunc(display);

    //Do drawing here
    grid = new Color[WIDTH * HEIGHT];
    for (GLint x = 0; x < WIDTH * HEIGHT; ++x)
    {
        grid[x] = 0;
    }
}
