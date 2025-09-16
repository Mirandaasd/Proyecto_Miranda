#include <GL/freeglut.h>
#include <iostream>
using namespace std;

// Tamaño de la ventana
int width = 800, height = 600;
bool showGrid = true;
bool showAxes = true;

// --- Dibujar un pixel ---
void putPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// --- Dibujar cuadrícula ---
void drawGrid(int spacing = 20) {
    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int x = -width/2; x <= width/2; x += spacing) {
        glVertex2i(x, -height/2);
        glVertex2i(x, height/2);
    }
    for (int y = -height/2; y <= height/2; y += spacing) {
        glVertex2i(-width/2, y);
        glVertex2i(width/2, y);
    }
    glEnd();
}

// --- Dibujar ejes ---
void drawAxes() {
    glColor3f(0, 0, 0);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // Eje X
    glVertex2i(-width/2, 0);
    glVertex2i(width/2, 0);
    // Eje Y
    glVertex2i(0, -height/2);
    glVertex2i(0, height/2);
    glEnd();
}

// --- Display callback ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (showGrid) drawGrid();
    if (showAxes) drawAxes();

    glFlush();
}

// --- Inicializar OpenGL ---
void init() {
    glClearColor(1, 1, 1, 1);          // Fondo blanco
    glColor3f(0, 0, 0);                // Color negro por defecto
    glPointSize(2);                    // Grosor de puntos
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-width/2, width/2, -height/2, height/2); // Sistema ortográfico 2D
}

// --- Menú popup ---
void menuOpciones(int opcion) {
    switch (opcion) {
        case 1: showGrid = !showGrid; break;
        case 2: showAxes = !showAxes; break;
        case 3: glClear(GL_COLOR_BUFFER_BIT); break;
        case 4: exit(0);
    }
    glutPostRedisplay();
}

// --- Main ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Proyecto CAD 2D - Base");

    init();

    // Crear menú
    int menu = glutCreateMenu(menuOpciones);
    glutAddMenuEntry("Mostrar/Ocultar cuadrícula", 1);
    glutAddMenuEntry("Mostrar/Ocultar ejes", 2);
    glutAddMenuEntry("Limpiar", 3);
    glutAddMenuEntry("Salir", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}

