#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
using namespace std;

int width = 800, height = 600;
bool showGrid = true;
bool showAxes = true;

// Algoritmo actual
// 1 = Recta Directa, 2 = Recta DDA, 3 = Círculo Incremental
int currentAlgorithm = 1;

// Variables generales
int clickCount = 0;
int x1_shape, y1_shape, x2_shape, y2_shape;
bool hasShape = false;

// --- Dibujar un pixel ---
void putPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// --- Dibujar 8 octantes de un círculo ---
void drawCirclePoints(int xc, int yc, int x, int y) {
    putPixel(xc + x, yc + y);
    putPixel(xc - x, yc + y);
    putPixel(xc + x, yc - y);
    putPixel(xc - x, yc - y);
    putPixel(xc + y, yc + x);
    putPixel(xc - y, yc + x);
    putPixel(xc + y, yc - x);
    putPixel(xc - y, yc - x);
}

// --- Algoritmo: Línea Directa ---
void lineaDirecta(int x1, int y1, int x2, int y2) {
    float m, b;
    if (x1 == x2) { // vertical
        for (int y = min(y1,y2); y <= max(y1,y2); y++)
            putPixel(x1, y);
    }
    else if (y1 == y2) { // horizontal
        for (int x = min(x1,x2); x <= max(x1,x2); x++)
            putPixel(x, y1);
    }
    else {
        m = (float)(y2 - y1) / (x2 - x1);
        b = y1 - m * x1;
        if (fabs(m) <= 1) {
            for (int x = min(x1,x2); x <= max(x1,x2); x++) {
                int y = round(m * x + b);
                putPixel(x, y);
            }
        } else {
            for (int y = min(y1,y2); y <= max(y1,y2); y++) {
                int x = round((y - b) / m);
                putPixel(x, y);
            }
        }
    }
}

// --- Algoritmo: Línea DDA ---
void lineaDDA(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = max(abs(dx), abs(dy));
    float x_inc = dx / (float) steps;
    float y_inc = dy / (float) steps;
    float x = x1;
    float y = y1;
    for (int i = 0; i <= steps; i++) {
        putPixel(round(x), round(y));
        x += x_inc;
        y += y_inc;
    }
}

// --- Algoritmo: Círculo Incremental por octantes ---
void circuloIncremental(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    while (x <= y) {
        drawCirclePoints(xc, yc, x, y);
        x++;
        y = (int) round(sqrt(r * r - x * x));
    }
}

// --- Display ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (showGrid) {
        glColor3f(0.9f, 0.9f, 0.9f);
        glBegin(GL_LINES);
        for (int x = -width/2; x <= width/2; x += 20) {
            glVertex2i(x, -height/2);
            glVertex2i(x, height/2);
        }
        for (int y = -height/2; y <= height/2; y += 20) {
            glVertex2i(-width/2, y);
            glVertex2i(width/2, y);
        }
        glEnd();
    }

    if (showAxes) {
        glColor3f(0, 0, 0);
        glLineWidth(2.0f);
        glBegin(GL_LINES);
        glVertex2i(-width/2, 0);
        glVertex2i(width/2, 0);
        glVertex2i(0, -height/2);
        glVertex2i(0, height/2);
        glEnd();
    }

    if (hasShape) {
        glColor3f(1, 0, 0); // rojo

        if (currentAlgorithm == 1)
            lineaDirecta(x1_shape, y1_shape, x2_shape, y2_shape);
        else if (currentAlgorithm == 2)
            lineaDDA(x1_shape, y1_shape, x2_shape, y2_shape);
        else if (currentAlgorithm == 3) {
            int r = (int) round(sqrt(pow(x2_shape - x1_shape, 2) + pow(y2_shape - y1_shape, 2)));
            circuloIncremental(x1_shape, y1_shape, r);
        }
    }

    glFlush();
}

// --- Mouse ---
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        int x_gl = x - width/2;
        int y_gl = height/2 - y;

        if (clickCount == 0) {
            x1_shape = x_gl;
            y1_shape = y_gl;
            clickCount = 1;
            hasShape = false;
            cout << "Punto inicial: (" << x1_shape << "," << y1_shape << ")\n";
        }
        else {
            x2_shape = x_gl;
            y2_shape = y_gl;
            clickCount = 0;
            hasShape = true;
            cout << "Punto final: (" << x2_shape << "," << y2_shape << ")\n";
        }

        glutPostRedisplay();
    }
}

// --- Inicializar OpenGL ---
void init() {
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 0, 0);
    glPointSize(2);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width/2, width/2, -height/2, height/2, -1, 1);
}

// --- Menús ---
void menuOpciones(int opcion) {
    switch (opcion) {
        case 1: showGrid = !showGrid; break;
        case 2: showAxes = !showAxes; break;
        case 3: glClear(GL_COLOR_BUFFER_BIT); hasShape = false; break;
        case 4: exit(0);
    }
    glutPostRedisplay();
}

void menuLineas(int opcion) {
    currentAlgorithm = opcion;
    cout << "Algoritmo seleccionado: " << (opcion == 1 ? "Recta Directa" : "Recta DDA") << "\n";
    glutPostRedisplay();
}

void menuCirculos(int opcion) {
    currentAlgorithm = opcion;
    cout << "Algoritmo seleccionado: Círculo Incremental\n";
    glutPostRedisplay();
}

// --- Main ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Proyecto CAD 2D - Lineas y Círculos");

    init();

    // Submenús
    int submenuLineas = glutCreateMenu(menuLineas);
    glutAddMenuEntry("Recta Directa", 1);
    glutAddMenuEntry("Recta DDA", 2);

    int submenuCirculos = glutCreateMenu(menuCirculos);
    glutAddMenuEntry("Círculo Incremental", 3);

    // Menú principal
    int menu = glutCreateMenu(menuOpciones);
    glutAddSubMenu("Dibujo de Líneas", submenuLineas);
    glutAddSubMenu("Dibujo de Círculos", submenuCirculos);
    glutAddMenuEntry("Mostrar/Ocultar cuadrícula", 1);
    glutAddMenuEntry("Mostrar/Ocultar ejes", 2);
    glutAddMenuEntry("Limpiar", 3);
    glutAddMenuEntry("Salir", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutDisplayFunc(display);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}
