# 🧑‍🚀 The Spacesuit Black-Box Decrypter (Mission: Ares-V)

### [Español]
Herramienta de ingeniería de sistemas a bajo nivel diseñada para la recuperación, desencefalamiento y modificación de datos de telemetría crítica de trajes espaciales en entornos hostiles (Exoplaneta Ares-V). 

### [English]
Low-level systems engineering tool designed for retrieving, parsing, and modifying critical spacesuit telemetry data in harsh environments (Exoplanet Ares-V).

---

## 🌌 El Contexto / The Lore
Cuando un colono regresa de una misión de exploración exterior en Ares-V, los trajes espaciales graban de forma compacta las constantes vitales (O2, temperatura, ID de usuario y Checksum de integridad) en un archivo binario crudo (`.bin`). Debido a la radiación cósmica, estos archivos suelen corromperse. Este software actúa como la herramienta de diagnóstico de emergencia para los ingenieros de la base: lee los bytes directamente del hardware, extrae la telemetría vital y permite reparar o calibrar los parámetros directamente en el archivo binario.

---

## 🛠️ Objetivos Técnicos Demostrados (Technical Skills)
Este miniproyecto abre mi portfolio demostrando mis fundamentos de ciencias de la computación y mi capacidad para trabajar cerca del sistema operativo sin abstracciones ni frameworks:

*   **Manipulación de Archivos Binarios:** Uso avanzado de funciones nativas de C (`fopen`, `fread`, `fwrite`, `fseek`) para mover el puntero del sistema de archivos a posiciones de memoria específicas.
*   **Gestión de Memoria y Estructuras:** Diseño de estructuras de datos (`struct`) alineadas a nivel de bytes para mapear el hardware del traje espacial de forma eficiente.
*   **Aritmética de Punteros y Bits:** Modificación directa de registros y bytes específicos en disco (Inyección/Hackeo de parámetros de simulación).
*   **Optimización de Recursos:** Software escrito en C Puro, libre de fugas de memoria (*memory leaks*), diseñado para ejecutarse con el menor consumo de CPU y RAM posible.

---

## 📂 Estructura del Archivo Binario (Data Layout)
El software interpreta el archivo `.bin` utilizando el siguiente mapeo de memoria estructural:

| Desplazamiento (Offset) | Tipo de Dato | Variable | Descripción |
| :--- | :--- | :--- | :--- |
| `0x00` | `char` (1 byte) | `nivel_oxigeno` | Porcentaje de O2 restante (0-100). |
| `0x01` | `char` (1 byte) | `temperatura` | Temperatura interna en grados Celsius. |
| `0x02` | `int` (4 bytes) | `codigo_colono` | ID único de identificación del astronauta. |

---

## 🚀 Instalación y Ejecución Rápida

Para compilar y ejecutar el decodificador en entornos basados en Linux/MacOS (o MinGW en Windows), abre la terminal en la carpeta del proyecto y ejecuta:

```bash
# 1. Compilar el código fuente con GCC
gcc decrypter.c -o decrypter

# 2. Ejecutar el software de diagnóstico espacial
./decrypter