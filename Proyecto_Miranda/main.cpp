#include <GL/freeglut.h>
#include <iostream>
#include <cmath>
using namespace std;

int width = 800, height = 600;
bool showGrid = true;
bool showAxes = true;

// Algoritmo actual de dibujo
// 1 = Directo, 2 = DDA
int currentAlgorithm = 1;

// Variables para dibujar líneas
int clickCount = 0;
int x1_line, y1_line, x2_line, y2_line;
bool hasLine = false; // bandera: ya tengo una línea lista

// --- Dibujar un pixel ---
void putPixel(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// --- Dibujar cuadrícula ---
void drawGrid(int spacing = 20) {
    glColor3f(0.9f, 0.9f, 0.9f);
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
    glVertex2i(-width/2, 0);
    glVertex2i(width/2, 0);
    glVertex2i(0, -height/2);
    glVertex2i(0, height/2);
    glEnd();
}

// --- Algoritmo: Línea método directo ---
void lineaDirecta(int x1, int y1, int x2, int y2) {
    float m, b;

    if (x1 == x2) { // Línea vertical
        for (int y = min(y1,y2); y <= max(y1,y2); y++)
            putPixel(x1, y);
    }
    else if (y1 == y2) { // Línea horizontal
        for (int x = min(x1,x2); x <= max(x1,x2); x++)
            putPixel(x, y1);
    }
    else {
        m = (float)(y2 - y1) / (x2 - x1);
        b = y1 - m * x1;

        if (fabs(m) <= 1) { // Avanzamos en X
            for (int x = min(x1,x2); x <= max(x1,x2); x++) {
                int y = round(m * x + b);
                putPixel(x, y);
            }
        }
        else { // Avanzamos en Y
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

// --- Display ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (showGrid) drawGrid();
    if (showAxes) drawAxes();

    // Dibujar la línea si ya hay una definida
    if (hasLine) {
        glColor3f(1, 0, 0); // rojo

        if (currentAlgorithm == 1)
            lineaDirecta(x1_line, y1_line, x2_line, y2_line);
        else if (currentAlgorithm == 2)
            lineaDDA(x1_line, y1_line, x2_line, y2_line);
    }

    glFlush();
}

// --- Mouse callback ---
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Transformar coordenadas de pantalla a coordenadas OpenGL
        int x_gl = x - width/2;
        int y_gl = height/2 - y;

        if (clickCount == 0) {
            x1_line = x_gl;
            y1_line = y_gl;
            clickCount = 1;
            hasLine = false;
            cout << "Punto inicial: (" << x1_line << ", " << y1_line << ")\n";
        }
        else {
            x2_line = x_gl;
            y2_line = y_gl;
            clickCount = 0;
            hasLine = true;
            cout << "Punto final: (" << x2_line << ", " << y2_line << ")\n";
        }

        glutPostRedisplay(); // refrescar pantalla
    }
}

// --- Inicializar OpenGL ---
void init() {
    glClearColor(1, 1, 1, 1);
    glColor3f(0, 0, 0);
    glPointSize(3); // tamaño de los puntos más grande
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width/2, width/2, -height/2, height/2, -1, 1);
}

// --- Menú principal ---
void menuOpciones(int opcion) {
    switch (opcion) {
        case 1: showGrid = !showGrid; break;
        case 2: showAxes = !showAxes; break;
        case 3: glClear(GL_COLOR_BUFFER_BIT); hasLine = false; break;
        case 4: exit(0);
    }
    glutPostRedisplay();
}

// --- Submenú: Algoritmos de línea ---
void menuLineas(int opcion) {
    currentAlgorithm = opcion;
    if (opcion == 1) cout << "Algoritmo seleccionado: Recta Directa\n";
    if (opcion == 2) cout << "Algoritmo seleccionado: Recta DDA\n";
    glutPostRedisplay();
}

// --- Main ---
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Proyecto CAD 2D - Lineas Directa y DDA");

    init();

    // Submenú para algoritmos de línea
    int submenuLineas = glutCreateMenu(menuLineas);
    glutAddMenuEntry("Recta Directa", 1);
    glutAddMenuEntry("Recta DDA", 2);

    // Menú principal
    int menu = glutCreateMenu(menuOpciones);
    glutAddSubMenu("Dibujo de Lineas", submenuLineas);
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

