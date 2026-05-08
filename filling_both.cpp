#include <GL/glut.h>
#include <iostream>
using namespace std;

static const int W = 500, H = 500;
unsigned char frame[H][W];  
// 0 = bg (white), 1 = boundary (black), 2 = fill (red)

int method = 0; // 0 = Flood, 1 = Boundary

// Rectangle bounds
int X1 = 100, Y1 = 100, X2 = 400, Y2 = 400;

// Set up initial frame: boundary and background
void initFrame() {
    for (int y = 0; y < H; ++y)
      for (int x = 0; x < W; ++x)
        frame[y][x] = 0;
    // Draw rectangle boundary in buffer
    for (int x = X1; x <= X2; ++x) {
      frame[Y1][x] = 1;
      frame[Y2][x] = 1;
    }
    for (int y = Y1; y <= Y2; ++y) {
      frame[y][X1] = 1;
      frame[y][X2] = 1;
    }
}

// Draw the buffer to screen
void renderFrame() {
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (int y = 0; y < H; ++y) {
      for (int x = 0; x < W; ++x) {
        if (frame[y][x] == 1)      glColor3f(0,0,0);   // boundary: black
        else if (frame[y][x] == 2) glColor3f(1,0,0);   // fill: red
        else                       glColor3f(1,1,1);   // bg: white
        glVertex2i(x, y);
      }
    }
    glEnd();
    glFlush();
}

// 4‑way flood fill on buffer
void floodFillBuf(int x, int y) {
    if (x < 0 || x >= W || y < 0 || y >= H) return;
    if (frame[y][x] != 0) return;  // stop at boundary or already filled
    frame[y][x] = 2;
    floodFillBuf(x+1,y); floodFillBuf(x-1,y);
    floodFillBuf(x,y+1); floodFillBuf(x,y-1);
}

// 4‑way boundary fill on buffer
void boundaryFillBuf(int x, int y) {
    if (x < 0 || x >= W || y < 0 || y >= H) return;
    if (frame[y][x] == 1 || frame[y][x] == 2) return;
    frame[y][x] = 2;
    boundaryFillBuf(x+1,y); boundaryFillBuf(x-1,y);
    boundaryFillBuf(x,y+1); boundaryFillBuf(x,y-1);
}

void display() {
    renderFrame();
}

void mouse(int button, int state, int mx, int my) {
    if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
        int y = my;          // GLUT y already inverted in our ortho
        int x = mx;
        if (x<=X1||x>=X2||y<=Y1||y>=Y2) return;
        if (method == 0) floodFillBuf(x,y);
        else              boundaryFillBuf(x,y);
        glutPostRedisplay();
    }
}

void menu(int opt) {
    if (opt==1) method=0;
    if (opt==2) method=1;
    if (opt==3) exit(0);
}

void init() {
    initFrame();
    gluOrtho2D(0, W, 0, H);
    glClearColor(1,1,1,1);
    glPointSize(1.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(W,H);
    glutCreateWindow("Flood & Boundary Fill (Buffer)");

    init();

    glutCreateMenu(menu);
      glutAddMenuEntry("Flood Fill",1);
      glutAddMenuEntry("Boundary Fill",2);
      glutAddMenuEntry("Exit",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}

