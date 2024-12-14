#include <GL/glut.h>
#include <GL/glu.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

// Puntos y caras
std::vector<std::vector<float>> laserPoints3D;
std::vector<std::vector<int>> faces;
float centroid[3] = {0.0, 0.0, 0.0};
float scale = 1.0;

// Variables de cámara
float rotateX = 0.0, rotateY = 0.0;
float zoom = 1.0;
int prevMouseX, prevMouseY;
bool leftMouseButtonDown = false;

// Función para leer el archivo OBJ
void readPoints(const std::string &filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Error al abrir el archivo " << filename << std::endl;
		return;
	}
	
	std::string line;
	float minX = std::numeric_limits<float>::max(), minY = minX, minZ = minX;
	float maxX = std::numeric_limits<float>::lowest(), maxY = maxX, maxZ = maxX;
	
	while (std::getline(file, line)) {
		if (line.rfind("v ", 0) == 0) {
			std::istringstream iss(line.substr(2));
			float x, y, z;
			if (iss >> x >> y >> z) {
				laserPoints3D.push_back({x, y, z});
				minX = std::min(minX, x); maxX = std::max(maxX, x);
				minY = std::min(minY, y); maxY = std::max(maxY, y);
				minZ = std::min(minZ, z); maxZ = std::max(maxZ, z);
			}
		} else if (line.rfind("f ", 0) == 0) {
			std::istringstream iss(line.substr(2));
			std::vector<int> face;
			int index;
			while (iss >> index) {
				face.push_back(index);
			}
			faces.push_back(face);
		}
	}
	
	// Calcular el centroide
	centroid[0] = (minX + maxX) / 2.0f;
	centroid[1] = (minY + maxY) / 2.0f;
	centroid[2] = (minZ + maxZ) / 2.0f;
	
	// Calcular la escala
	float maxDistance = std::max({maxX - minX, maxY - minY, maxZ - minZ});
	scale = 2.0f / maxDistance; // Ajuste de escala para que quepa en la pantalla
}

// Función de renderización
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0 / zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	// Aplicar rotaciones
	glRotatef(rotateX, 1.0, 0.0, 0.0);
	glRotatef(rotateY, 0.0, 1.0, 0.0);
	
	// Dibujar las caras
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0); // Rojo para las caras
	for (const auto &face : faces) {
		for (int index : face) {
			glVertex3f((laserPoints3D[index - 1][0] - centroid[0]) * scale,
				(laserPoints3D[index - 1][1] - centroid[1]) * scale,
				(laserPoints3D[index - 1][2] - centroid[2]) * scale);
		}
	}
	glEnd();
	
	// Dibujar bordes de los triángulos
	glLineWidth(1.0f);
	glColor3f(1.0, 1.0, 1.0); // Blanco para los bordes
	glBegin(GL_LINES);
	for (const auto &face : faces) {
		for (size_t i = 0; i < face.size(); ++i) {
			int startIdx = face[i] - 1;
			int endIdx = face[(i + 1) % face.size()] - 1;
			glVertex3f((laserPoints3D[startIdx][0] - centroid[0]) * scale,
				(laserPoints3D[startIdx][1] - centroid[1]) * scale,
				(laserPoints3D[startIdx][2] - centroid[2]) * scale);
			glVertex3f((laserPoints3D[endIdx][0] - centroid[0]) * scale,
				(laserPoints3D[endIdx][1] - centroid[1]) * scale,
				(laserPoints3D[endIdx][2] - centroid[2]) * scale);
		}
	}
	glEnd();
	
	glutSwapBuffers();
}

// Inicializar OpenGL
void initOpenGL() {
	glEnable(GL_DEPTH_TEST);
	glPointSize(5.0f);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

// Ajustar la perspectiva al redimensionar
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

// Control del mouse para rotación
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			leftMouseButtonDown = true;
			prevMouseX = x;
			prevMouseY = y;
		} else if (state == GLUT_UP) {
			leftMouseButtonDown = false;
		}
	} else if (button == 3 || button == 4) { // Scroll del mouse
		zoom += (button == 3) ? 0.1f : -0.1f;
		if (zoom < 0.1f) zoom = 0.1f; // Evitar zoom negativo
		glutPostRedisplay();
	}
}

// Movimiento del mouse para rotación
void motion(int x, int y) {
	if (leftMouseButtonDown) {
		rotateY += (x - prevMouseX) * 0.5f;
		rotateX += (y - prevMouseY) * 0.5f;
		prevMouseX = x;
		prevMouseY = y;
		glutPostRedisplay();
	}
}

// Función de temporizador
void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}

// Función principal
int main(int argc, char **argv) {
	std::string filename = "C:\\Users\\SERGIO\\Desktop\\Clones\\Scanner3D\\high_density_torus.txt"; 
	//C:\\Users\\SERGIO\\Desktop\\Clones\\Scanner3D\\high_density_sphere.txt
	//C:\\Users\\SERGIO\\Desktop\\Clones\\Scanner3D\\Esfera.txt
	//C:\\Users\\SERGIO\\Desktop\\Clones\\Scanner3D\\high_density_torus.txt
	readPoints(filename);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("3D Viewer with Zoom and Improved Visualization");
	initOpenGL();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutTimerFunc(0, timer, 0);
	glutMainLoop();
	
	return 0;
}

