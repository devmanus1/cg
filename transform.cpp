#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

// Bigger triangle vertices (pre-drawn in the first quadrant)
float x[3] = {50.0, 100.0, 75.0}, y[3] = {50.0, 50.0, 86.6}; // Larger triangle coordinates

void drawTriangle() {
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 3; i++) glVertex2f(x[i], y[i]);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw axes
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(-320, 0); glVertex2f(320, 0);  // X axis
    glVertex2f(0, -240); glVertex2f(0, 240);  // Y axis
    glEnd();

    // Draw large triangle in white color
    glColor3f(1, 1, 1); // Set color to white
    drawTriangle();
    glFlush();
}

void scale() {
    float sx, sy;
    printf("\nEnter scale factors (x y): ");
    scanf("%f %f", &sx, &sy);
    for(int i = 0; i < 3; i++) {
        x[i] *= sx; y[i] *= sy;
    }
    glutPostRedisplay();
}

void rotate() {
    float angle;
    printf("\nEnter rotation angle in degrees: ");
    scanf("%f", &angle);

    float rad = angle * M_PI / 180;
    float temp_x, temp_y;

    // Rotation around the left-bottom corner (50, 50)
    for(int i = 0; i < 3; i++) {
        // Translate point to origin
        temp_x = x[i] - x[0];
        temp_y = y[i] - y[0];
        
        // Perform rotation
        x[i] = x[0] + (temp_x * cos(rad) - temp_y * sin(rad));
        y[i] = y[0] + (temp_x * sin(rad) + temp_y * cos(rad));
    }
    glutPostRedisplay();
}

void reflect() {
    int choice;
    printf("\n1. Reflect about X axis\n2. Reflect about Y axis\nEnter choice: ");
    scanf("%d", &choice);
    for(int i = 0; i < 3; i++) {
        if (choice == 1) y[i] = -y[i];  // Reflect about X axis
        else x[i] = -x[i];              // Reflect about Y axis
    }
    glutPostRedisplay();
}

void menu(int choice) {
    switch(choice) {
        case 1: scale(); break;
        case 2: rotate(); break;
        case 3: reflect(); break;
        case 4: exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutCreateWindow("2D Transformations");

    glClearColor(0, 0, 0, 0); // Set background to black
    gluOrtho2D(-320, 320, -240, 240); // Set the orthographic projection

    glutDisplayFunc(display);
    glutCreateMenu(menu);
    glutAddMenuEntry("Scale", 1);
    glutAddMenuEntry("Rotate", 2);
    glutAddMenuEntry("Reflect", 3);
    glutAddMenuEntry("Exit", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}

