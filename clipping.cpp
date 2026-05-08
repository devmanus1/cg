#include <GL/glut.h>
#include <iostream>
using namespace std;

int xmin = -100, ymin = -100, xmax = 100, ymax = 100;
float x_1, y_1, x_2, y_2;
int clicks = 0;

const int INSIDE = 0, LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8;

int getCode(float x, float y) {
    int code = INSIDE;
    if (x < xmin) code |= LEFT;
    else if (x > xmax) code |= RIGHT;
    if (y < ymin) code |= BOTTOM;
    else if (y > ymax) code |= TOP;
    return code;
}

void cohenSutherlandClip() {
    int code1 = getCode(x_1, y_1);
    int code2 = getCode(x_2, y_2);
    bool accept = false;

    while (true) {
        if ((code1 | code2) == 0) {
            accept = true;
            break;
        } else if (code1 & code2) {
            break;
        } else {
            int outcode = code1 ? code1 : code2;
            float x, y;

            if (outcode & TOP) {
                x = x_1 + (x_2 - x_1) * (ymax - y_1) / (y_2 - y_1);
                y = ymax;
            } else if (outcode & BOTTOM) {
                x = x_1 + (x_2 - x_1) * (ymin - y_1) / (y_2 - y_1);
                y = ymin;
            } else if (outcode & RIGHT) {
                y = y_1 + (y_2 - y_1) * (xmax - x_1) / (x_2 - x_1);
                x = xmax;
            } else {
                y = y_1 + (y_2 - y_1) * (xmin - x_1) / (x_2 - x_1);
                x = xmin;
            }

            if (outcode == code1) {
                x_1 = x;
                y_1 = y;
                code1 = getCode(x_1, y_1);
            } else {
                x_2 = x;
                y_2 = y;
                code2 = getCode(x_2, y_2);
            }
        }
    }

    if (accept) glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Clipping rectangle
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2i(xmin, ymin);
    glVertex2i(xmax, ymin);
    glVertex2i(xmax, ymax);
    glVertex2i(xmin, ymax);
    glEnd();

    // Draw line
    glColor3f(0, 0, 1);
    glBegin(GL_LINES);
    glVertex2f(x_1, y_1);
    glVertex2f(x_2, y_2);
    glEnd();

    glFlush();
}

void mouse(int btn, int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float mx = x - 320;
        float my = 240 - y;

        if (clicks == 0) {
            x_1 = mx;
            y_1 = my;
            clicks++;
        } else {
            x_2 = mx;
            y_2 = my;
            clicks = 0;
            glutPostRedisplay();
        }
    }
}

void key(unsigned char ch, int x, int y) {
    if (ch == 'c') cohenSutherlandClip();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(-320, 320, -240, 240);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Cohen-Sutherland Line Clipping");

    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(key);
    glutMainLoop();
    return 0;
}

