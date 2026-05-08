#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct Line {
    int x1, y1, x2, y2, algo, style;
};

vector<Line> lines;
int tempX, tempY;
bool waiting = false;
int currentAlgo = 0;  // 0 = DDA, 1 = Bresenham
int currentStyle = 0; // 0 = solid, 1 = dotted, 2 = dashed

void printState() {
    cout << (currentAlgo == 0 ? "DDA" : "Bresenham") << " : ";
    if (currentStyle == 0) cout << "Solid" << endl;
    else if (currentStyle == 1) cout << "Dotted" << endl;
    else cout << "Dashed" << endl;
}

void setPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void ddaLine(int x1, int y1, int x2, int y2, int style) {
    float dx = x2 - x1, dy = y2 - y1;
    float steps = max(abs(dx), abs(dy));
    float x = x1, y = y1, xinc = dx / steps, yinc = dy / steps;
    for (int i = 0; i <= steps; i++) {
        bool draw = true;
        if (style == 1 && i % 4 != 0) draw = false;
        if (style == 2 && (i / 5) % 2 == 1) draw = false;
        if (draw) setPixel(round(x), round(y));
        x += xinc;
        y += yinc;
    }
}

void bresenhamLine(int x1, int y1, int x2, int y2, int style) {
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;
    int err = dx - dy, i = 0;
    while (true) {
        bool draw = true;
        if (style == 1 && i % 4 != 0) draw = false;
        if (style == 2 && (i / 5) % 2 == 1) draw = false;
        if (draw) setPixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
        i++;
    }
}

void drawAxes() {
    glBegin(GL_LINES);
    glVertex2i(-250, 0); glVertex2i(250, 0);
    glVertex2i(0, -250); glVertex2i(0, 250);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();
    for (auto& l : lines) {
        if (l.algo == 0)
            ddaLine(l.x1, l.y1, l.x2, l.y2, l.style);
        else
            bresenhamLine(l.x1, l.y1, l.x2, l.y2, l.style);
    }
    glFlush();
}

void mouse(int btn, int state, int x, int y) {
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int cx = x - 250, cy = 250 - y;
        if (!waiting) {
            tempX = cx;
            tempY = cy;
            waiting = true;
        } else {
            Line l = {tempX, tempY, cx, cy, currentAlgo, currentStyle};
            lines.push_back(l);
            waiting = false;
            glutPostRedisplay();
        }
    }
}

void keyboard(unsigned char key, int, int) {
    if (key == 'd') currentAlgo = 0;
    if (key == 'b') currentAlgo = 1;
    if (key == '1') currentStyle = 0;
    if (key == '2') currentStyle = 1;
    if (key == '3') currentStyle = 2;
    printState();
}

void init() {
    gluOrtho2D(-250, 250, -250, 250);
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 0, 0);
    glPointSize(2);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Multiple Line Drawing");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}

