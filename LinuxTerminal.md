# 3D Model Viewer with OpenGL

Este proyecto es un visor de modelos 3D que utiliza OpenGL para renderizar figuras basadas en un archivo de texto con coordenadas 3D y definiciones de caras.

## Características
- Renderizado de figuras 3D utilizando OpenGL y GLUT.
- Escalado y centrado automático de los modelos en pantalla.
- Control interactivo de la cámara con el mouse (rotación y zoom).
- Compatible con archivos en formato OBJ simplificado.

## Requisitos

### Sistema Operativo
- Linux (probado en Debian y derivados).

### Dependencias
Asegúrate de tener instalados los siguientes paquetes:

```bash
sudo apt update
sudo apt install build-essential freeglut3-dev libglu1-mesa-dev
```

## Configuración y Ejecución

### Clonar el repositorio

```bash
git clone https://github.com/<tu_usuario>/Scanner3D.git
cd Scanner3D
```

### Compilación
Usa el siguiente comando para compilar el programa:

```bash
g++ -fshow-column -fno-diagnostics-show-caret -g modelado.cpp \
    -Wall -pedantic-errors -O0 -Werror=return-type -std=c++14 \
    -o modelado.bin -lGL -lGLU -lglut


#caso mio
g++ -fshow-column -fno-diagnostics-show-caret -g /home/sergio9/github/Scanner3D/modelado.cpp \
    -Wall -pedantic-errors -O0 -Werror=return-type -std=c++14 -o /home/sergio9/github/Scanner3D/modelado.bin \
    -lGL -lGLU -lglut

#luego
/home/sergio9/github/Scanner3D/modelado.bin
```

Esto generará el ejecutable `modelado.bin` en el directorio del proyecto.

### Ejecución

Ejecuta el programa indicando el archivo del modelo como argumento:

```bash
./modelado.bin <ruta_al_archivo_modelo>
```

Por ejemplo:

```bash
./modelado.bin /home/usuario/github/Scanner3D/new_figure.txt
```

## Modificación del Modelo 3D
Si deseas cambiar la figura renderizada, proporciona un archivo con formato OBJ simplificado, como este:

```obj
v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 0.0 1.0 0.0
f 1 2 3
```

Guarda este archivo en una ubicación accesible y pasa su ruta como argumento al ejecutar el programa.

## Estructura del Código

### Archivos principales
- `modelado.cpp`: Contiene el código fuente principal.
- `new_figure.txt`: Archivo de ejemplo con datos para modelar figuras.

### Configuración del archivo de entrada
El archivo de entrada se puede especificar como argumento en la línea de comandos. Esto elimina la necesidad de recompilar el código cada vez que cambies de modelo.

Ejemplo:
```cpp
std::string filename;
if (argc > 1) {
    filename = argv[1];
} else {
    std::cerr << "Uso: " << argv[0] << " <archivo_modelo.obj>" << std::endl;
    return 1;
}
```

## Controles

- **Rotación**: Mantén presionado el botón izquierdo del mouse y arrastra para rotar el modelo.
- **Zoom**: Usa la rueda del mouse para acercar o alejar.

## Troubleshooting

### Error: `undefined reference to ...`
Este error ocurre si faltan bibliotecas durante la compilación. Asegúrate de incluir `-lGL -lGLU -lglut` al final del comando de compilación.

### Error: "No such file or directory"
Verifica que el archivo especificado exista y que la ruta sea correcta.

### Problema con el escalado o el centrado del modelo
El programa ajusta automáticamente la escala y el centro del modelo. Asegúrate de que las coordenadas en el archivo de entrada estén correctamente definidas.

## Licencia
Este proyecto está bajo la Licencia MIT. Puedes utilizarlo y modificarlo libremente. ¡Agradecemos contribuciones y mejoras!


