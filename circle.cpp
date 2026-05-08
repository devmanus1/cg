#include <GL/glut.h>
#include <stdio.h>

int r;

void plot(int xc, int yc, int x, int y) {
    glVertex2i(xc + x, yc + y); glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y); glVertex2i(xc - x, yc - y);
    glVertex2i(xc + y, yc + x); glVertex2i(xc - y, yc + x);
    glVertex2i(xc + y, yc - x); glVertex2i(xc - y, yc - x);
}

void circle(int xc, int yc, int r) {
    int x = 0, y = r, d = 3 - 2 * r;
    while (x <= y) {
        plot(xc, yc, x, y);
        if (d < 0)
            d += 4 * x + 6;
        else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

void drawAxes() {
    glBegin(GL_LINES);
    glVertex2i(-300, 0); glVertex2i(300, 0); // X-axis
    glVertex2i(0, -300); glVertex2i(0, 300); // Y-axis
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0, 0);
    drawAxes();
    glBegin(GL_POINTS);
    circle(100, 100, r);
    circle(-100, 100, r);
    circle(-100, -100, r);
    circle(100, -100, r);
    glEnd();
    glFlush();
}

void init() {
    gluOrtho2D(-300, 300, -300, 300);
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 0, 0);
}

int main(int argc, char **argv) {
    printf("Enter radius: ");
    scanf("%d", &r);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Bresenham Circle with Quadrants");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

