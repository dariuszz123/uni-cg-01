#include "cmath"
#include "GL/glut.h"

int current = 7;
int draw_type = 1;

struct table {
    double a, b, c, d, e, f;
    int p;
    float r, z, m;
};

static struct table tables[] = {
        {0.5,  0.0,  0.0,  0.5,  0.0,  0.5, 4, 1, 0, 0},
        {-0.5,  0.0,  0.0, 0.5,  1.0,  0.0, 4, 0, 1, 0 },
        { 0.0,  0.5,  -0.5,  0.0,  0.0,  0.5, 4, 0, 0 ,1 },
        { 0.0,  0.25,  0.25,  0.0,  0.5,  0.75, 1, 1, 1, 1 }
};

static double xx = 0.0;
static double yy = 0.0;

static double maxX = 0.0;
static double minX = 0.0;
static double maxY = 0.0;
static double minY = 0.0;

static int points = 0;

struct table get_table() {
    int r = rand() % 13;
    int sum = 0;
    int x = 0;
    for (; x < 4; x++) {
        sum += tables[x].p;
        if (sum > r) {
            return tables[x];
        }
    }

    return tables[0];
}

void draw_points() {
    struct table v = get_table();

    double x = xx * v.a + yy * v.b + v.e;
    double y = xx * v.c + yy * v.d + v.f;

    xx = x;
    yy = y;

    if (x > maxX) maxX = x;
    if (x < minX) minX = x;
    if (y > maxY) maxY = y;
    if (y < minY) minY = y;

    double dx = maxX - minX;
    double dy = maxY - minY;

    double k = dx > dy ? dx : dy;

    double realX = x / k * 2 - 1;
    double realY = y / k * 2 - 1;

    if (points > 300) {
        glColor3f(v.r, v.z, v.m);
        glVertex2f(realX, realY);
    }
}

void drawBasic() {
    glRectf(-1, -1, -0.5, 1);
    glRectf(-1, -1, 1, -0.6);
}

void upRightTransform() {
    glRotatef(90, 0, 0, 1);
    glRotatef(180, 1, 0, 0);
    glScalef(0.25, 0.25, 0.0);
    glTranslatef(3, 1, 0.0);
}

void upLeftTransform() {
    glScalef(0.5, 0.5, 1.0);
    glTranslatef(-1, 1, 0.0);
}

void bottomRightTransform() {
    glRotatef(180, 0, 1, 0);
    glScalef(0.5, 0.5, 0.0);
    glTranslatef(0, 0, 0.0);
    glTranslatef(-1, -1, 0.0);
}

void bottomLeftTransform() {
    glRotatef(270, 0, 0, 1);
    glScalef(0.5, 0.5, 0.0);
    glTranslatef(1, -1, 0.0);
}

void color(int num, int color) {
    if (num == current) {
        switch (color) {
            default:
                glColor3f(1.0, 1.0, 1.0);
                break;
            case 1:
                glColor3f(1.0, 0.0, 0.0);
                break;
            case 2:
                glColor3f(0.0, 1.0, 0.0);
                break;
            case 3:
                glColor3f(0.0, 0.0, 1.0);
                break;
        }
    }
}

void drawFractal(int num) {

    glutPostRedisplay();
    switch (num) {
        case 0:
            color(0, 0);
            drawBasic();
            break;

        default:
            glPushMatrix();
            glPushMatrix();
            glPushMatrix();

            upRightTransform();
            color(num, 0);
            drawFractal(num - 1);

            glPopMatrix();

            upLeftTransform();
            color(num, 1);
            drawFractal(num - 1);

            glPopMatrix();
            bottomLeftTransform();
            color(num, 2);
            drawFractal(num - 1);

            glPopMatrix();
            bottomRightTransform();
            color(num, 3);
            drawFractal(num - 1);

            break;
    }
}

void probabilityDraw() {
    int n = points + 500 * pow(2, current);
    glBegin(GL_POINTS);
    for (; points < n; points++)
        draw_points();
    glEnd();
    glFinish();
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (draw_type == 0) {
        drawFractal(current);
    } else {
        probabilityDraw();
    }

    glutSwapBuffers();
}

void resize(int w, int h) {
    glViewport(0, 0, w, h);

    draw();
}

void keyboard(unsigned char c, int x, int y) {
    current++;

    if (c == 't') {
        if (draw_type == 0) {
            draw_type = 1;
        } else {
            draw_type = 0;
        }

        current = 0;
    }

    draw();

    if (current > 7) {
        current = 0;
    }
}

void init() {
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    glClear(GL_COLOR_BUFFER_BIT);

    draw();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(440, 440);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Fractal");

    glutDisplayFunc(init);

    glutMainLoop();
    return 0;
}