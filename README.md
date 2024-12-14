# 3D Scanner code c++ con Zinjal 

Este repositorio contiene un visor 3D que renderiza modelos a partir de archivos OBJ utilizando OpenGL y GLUT. Permite visualizar y manipular modelos 3D con funciones como rotación y zoom. Es ideal para explorar representaciones geométricas y aprender cómo manejar archivos OBJ.

## Características

- Lectura de archivos OBJ para extraer puntos y caras 3D.
- Renderización de modelos 3D con soporte para rotación y zoom.
- Representación de triángulos con bordes y caras coloreadas.
- Escalado automático para ajustar el modelo al espacio de visualización.

## Requisitos

- OpenGL
- GLUT
- GLU

### Instalación

Para compilar y ejecutar este código, asegúrate de tener instaladas las librerías mencionadas. Usa un compilador como `g++`:

```bash
# En sistemas Linux o similares
sudo apt-get install freeglut3-dev

# Compilar el programa
g++ -o 3d_viewer main.cpp -lGL -lGLU -lglut

# Ejecutar
./3d_viewer
```

## Descripción del Código

### Archivos y Variables Principales

- **Archivo OBJ:** Se especifica en la variable `filename` dentro de `main`.
- **Estructuras principales:**
  - `std::vector<std::vector<float>> laserPoints3D`: Almacena los puntos 3D extraídos del archivo OBJ.
  - `std::vector<std::vector<int>> faces`: Contiene las caras del modelo representadas como índices de los puntos.
  - `float centroid[3]`: Centroide del modelo para centrarlo en la escena.
  - `float scale`: Factor de escala para ajustar el tamaño del modelo.

### Funciones

#### 1. Lectura de puntos desde un archivo OBJ
```cpp
void readPoints(const std::string &filename) {
    // Lógica principal para leer el archivo OBJ
}
```
**Propósito:**
- Leer y procesar los datos de puntos y caras de un archivo OBJ.

**Descripción detallada:**
1. Se abre el archivo utilizando `std::ifstream`.
2. Para cada línea:
   - Si comienza con "v ", se interpreta como un punto 3D.
   - Si comienza con "f ", se interpreta como una cara.
3. Se calculan los valores mínimos y máximos de las coordenadas (x, y, z).
4. Se determina el centroide y la escala para normalizar el modelo.

#### 2. Renderización de las caras del modelo
```cpp
glBegin(GL_TRIANGLES);
glColor3f(1.0, 0.0, 0.0);
for (const auto &face : faces) {
    for (int index : face) {
        glVertex3f((laserPoints3D[index - 1][0] - centroid[0]) * scale,
                   (laserPoints3D[index - 1][1] - centroid[1]) * scale,
                   (laserPoints3D[index - 1][2] - centroid[2]) * scale);
    }
}
glEnd();
```
**Propósito:** Dibujar las caras del modelo 3D.

**Detalles:**
- `glBegin(GL_TRIANGLES)`: Indica que se van a dibujar triángulos.
- `glColor3f(1.0, 0.0, 0.0)`: Configura el color rojo para las caras.
- Para cada cara:
  - Itera los índices de vértices de la cara y define los puntos ajustados por el centroide y escala.

#### 3. Renderización de bordes del modelo
```cpp
glBegin(GL_LINES);
glColor3f(1.0, 1.0, 1.0);
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
```
**Propósito:** Dibujar los bordes entre vértices de cada cara.

#### 4. Manejo de eventos del mouse
```cpp
void mouse(int button, int state, int x, int y) {
    // Lógica de zoom y rotación
}
```
**Propósito:**
- Gestionar interacciones de clic y scroll del mouse.
- Permitir zoom y rotación.

**Detalles:**
- Botón izquierdo: activa el modo de rotación.
- Scroll: ajusta el nivel de zoom.

#### 5. Función principal
```cpp
int main(int argc, char **argv) {
    // Configuración de GLUT y bucle principal
}
```
**Propósito:** Configurar y lanzar la aplicación.

**Detalles:**
1. Se leen los puntos del archivo OBJ.
2. Se inicializan las funciones y parámetros de OpenGL.
3. Se lanza el bucle principal de renderización.

## Estructura del Archivo OBJ
El archivo debe seguir el formato:
```obj
v x1 y1 z1
v x2 y2 z2
...
f i1 i2 i3
...
```
Donde:
- `v x y z` define un punto 3D.
- `f i1 i2 i3` define una cara utilizando índices de puntos.

## Ejemplo de Archivo OBJ
```obj
v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 0.0 1.0 0.0
f 1 2 3
```

