#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

int choice;
float bx[4] = {-150, -50, 50, 150};
float by[4] = {-100, 200, -200, 100};
int level = 3;

void getBezierPoint(float t, float *x, float *y) {
    float u = 1 - t;
    float u2 = u * u;
    float u3 = u2 * u;
    float t2 = t * t;
    float t3 = t2 * t;
    *x = u3 * bx[0] + 3 * u2 * t * bx[1] + 3 * u * t2 * bx[2] + t3 * bx[3];
    *y = u3 * by[0] + 3 * u2 * t * by[1] + 3 * u * t2 * by[2] + t3 * by[3];
}

void drawBezier(float t1, float t2, int depth) {
    if (depth == 0) {
        float x1, y1, x2, y2;
        getBezierPoint(t1, &x1, &y1);
        getBezierPoint(t2, &x2, &y2);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        return;
    }
    float mid1 = (2 * t1 + t2) / 3;
    float mid2 = (t1 + 2 * t2) / 3;
    drawBezier(t1, mid1, depth - 1);
    drawBezier(mid1, mid2, depth - 1);
    drawBezier(mid2, t2, depth - 1);
}

void drawKoch(float x1, float y1, float x2, float y2, int n) {
    if (n == 0) {
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        return;
    }
    float dx = (x2 - x1) / 3;
    float dy = (y2 - y1) / 3;
    float xa = x1 + dx;
    float ya = y1 + dy;
    float xb = x2 - dx;
    float yb = y2 - dy;
    float xm = (xa + xb) / 2 - sqrt(3) * (yb - ya) / 2;
    float ym = (ya + yb) / 2 + sqrt(3) * (xb - xa) / 2;
    drawKoch(x1, y1, xa, ya, n - 1);
    drawKoch(xa, ya, xm, ym, n - 1);
    drawKoch(xm, ym, xb, yb, n - 1);
    drawKoch(xb, yb, x2, y2, n - 1);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    gluOrtho2D(-320, 320, -240, 240);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(-320, 0);
    glVertex2f(320, 0);
    glVertex2f(0, -240);
    glVertex2f(0, 240);
    glEnd();
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    if (choice == 1) {
        drawBezier(0, 1, 4);
    } else {
        drawKoch(-200, -50, 200, -50, level);
    }
    glEnd();
    glFlush();
}

void menu(int item) {
    choice = item;
    glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(640, 480);
    glutCreateWindow("Fractals");
    glutDisplayFunc(display);
    glutCreateMenu(menu);
    glutAddMenuEntry("Bezier", 1);
    glutAddMenuEntry("Koch", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    return 0;
}