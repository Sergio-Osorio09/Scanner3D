import cv2
import numpy as np
import math


# Función para extraer la línea del láser de una imagen
def extraer_linea_laser(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    rojo_bajo = np.array([0, 100, 100])
    rojo_alto = np.array([10, 255, 255])
    mascara = cv2.inRange(hsv, rojo_bajo, rojo_alto)
    bordes = cv2.Canny(mascara, 50, 150)

    # Dilatar y erosionar para mejorar la detección
    kernel = np.ones((3, 3), np.uint8)
    bordes = cv2.dilate(bordes, kernel, iterations=1)
    bordes = cv2.erode(bordes, kernel, iterations=1)

    puntos_laser = np.column_stack(np.where(bordes > 0))
    return puntos_laser


# Parámetros de calibración de la cámara
# Estos parámetros deben ser ajustados de acuerdo con la calibración de tu cámara
mtx = np.array([[1000, 0, 320], [0, 1000, 240], [0, 0, 1]])
dist = np.zeros((5, 1))


# Función para capturar y procesar imágenes
def capturar_y_procesar_frames(pasos):
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error al abrir la cámara")
        return []

    puntos_laser_3d = []
    for i in range(pasos):
        ret, frame = cap.read()
        if not ret:
            print("Error al capturar el frame")
            break

        frame = cv2.undistort(frame, mtx, dist)
        linea_laser = extraer_linea_laser(frame)

        # Calcular coordenadas 3D
        for punto in linea_laser:
            x = punto[1] / frame.shape[1] * 2 - 1
            y = punto[0] / frame.shape[0] * 2 - 1
            angulo = i / pasos * 360
            z = math.sin(math.radians(angulo))

            puntos_laser_3d.append((x, y, z))

        cv2.imshow("Frame", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
    return puntos_laser_3d


# Guardar los puntos 3D y las caras en un archivo
def guardar_puntos_y_caras(puntos, nombre_archivo):
    with open(nombre_archivo, 'w') as f:
        for punto in puntos:
            f.write(f"v {punto[0]} {punto[1]} {punto[2]}\n")

        # Generar caras (triángulos) a partir de los puntos
        num_puntos = len(puntos)
        for i in range(num_puntos - 1):
            if i < num_puntos - 2:
                f.write(f"f {i + 1} {i + 2} {i + 3}\n")  # Triángulo
            if i == num_puntos - 2:  # Cerrar el ciclo
                f.write(f"f {i + 1} {1} {i + 2}\n")  # Triángulo que cierra el ciclo

def main():
    pasos = 360  # Número de pasos de rotación
    puntos_laser_3d = capturar_y_procesar_frames(pasos)
    guardar_puntos_y_caras(puntos_laser_3d, 'laser_points.txt')


if _name_ == "_main_":
    main()