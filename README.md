# Satellite-C · Spacesuit Telemetry Toolkit (Misión Ares-V)

Conjunto de herramientas en **C puro** y **Python** para el tratamiento de telemetría de trajes espaciales, sin frameworks ni dependencias externas. Incluye dos componentes independientes:

- **`decrypter`**: utilidad de consola que lee y modifica en caliente un registro binario crudo (`telemetria.bin`) usando acceso directo por *offset*.
- **`satellite_server` + `transmitter.py`**: pareja cliente/servidor sobre **sockets TCP** que transmite tramas binarias de telemetría y evalúa en tiempo real si las constantes vitales del colono son críticas.

El proyecto trabaja cerca del sistema operativo: estructuras empaquetadas a nivel de byte, E/S binaria con `fopen`/`fread`/`fwrite`/`fseek` y programación de sockets (Berkeley sockets / Winsock).

---

## 1. Características principales

- **Lectura y edición binaria por offset**: modifica un único byte del fichero (`fseek` + `fwrite` en modo `rb+`) sin reescribir el registro completo.
- **Autoinicialización**: si `telemetria.bin` no existe, se genera con valores de fábrica (O₂ 85 %, 22 °C, ID `99999`).
- **Terminal interactiva** con protocolos de emergencia: recarga de oxígeno al 100 % y calibración de temperatura a 25 °C.
- **Servidor TCP de telemetría** (puerto `8080`) con clasificación automática de estado:
  - `ALERTA CRÍTICA` si **O₂ < 20 %** o **temperatura > 40 °C**.
  - `Condiciones estables` en cualquier otro caso.
- **Cliente emisor en Python** que serializa la trama con `struct.pack` en formato binario compacto (little-endian).
- **Estructuras empaquetadas** (`__attribute__((packed))`) para un mapeo determinista de bytes, sin padding.
- **Multiplataforma**: el servidor compila en Linux, macOS y Windows (MinGW/Winsock) mediante compilación condicional.
- **Cero dependencias**: solo biblioteca estándar de C y de Python.

---

## 2. Tecnologías utilizadas

| Capa | Tecnología |
| :--- | :--- |
| Lenguaje principal | **C** (GCC, extensiones `__attribute__((packed))`) |
| Cliente de simulación | **Python 3** (`socket`, `struct`, `time`; solo *stdlib*) |
| Red | Sockets **TCP/IPv4** (POSIX `arpa/inet.h` · Winsock2 en Windows) |
| E/S de ficheros | API estándar de C (`stdio.h`): `fopen`, `fread`, `fwrite`, `fseek` |
| Toolchain | GCC / MinGW (x86) |
| Entorno de desarrollo | VS Code (`C/C++ Runner`, depuración con GDB) |

---

## 3. Estructura del proyecto

```text
Satellite-C/
├── decrypter.c            # Terminal de ingeniería: lectura/edición de telemetria.bin
├── satellite_server.c     # Servidor TCP receptor de telemetría (puerto 8080)
├── transmitter.py         # Cliente TCP emisor de telemetría simulada
├── telemetria.bin         # Registro binario (se autogenera si no existe)
├── decrypter.exe          # Binario Windows precompilado (PE32, x86)
├── satellite_server.exe   # Binario Windows precompilado (PE32, x86)
├── .vscode/               # Configuración de IntelliSense, depuración y warnings
└── README.md
```

### Formatos binarios

El proyecto maneja **dos formatos distintos**, ambos *little-endian* y sin *padding*.

**a) Registro en disco: `telemetria.bin`** (`decrypter.c`, 6 bytes)

| Offset | Tipo | Campo | Descripción |
| :--- | :--- | :--- | :--- |
| `0x00` | `char` (1 B) | `nivel_oxigeno` | Porcentaje de O₂ restante (0–100). |
| `0x01` | `char` (1 B) | `temperatura` | Temperatura interna en °C. |
| `0x02` | `int` (4 B) | `codigo_colono` | ID único del colono. |

**b) Trama de red TCP** (`satellite_server.c` / `transmitter.py`, 4 bytes)

| Offset | Tipo C | Formato `struct` (Python) | Campo |
| :--- | :--- | :--- | :--- |
| `0x00` | `char` (1 B) | `b` | `nivel_oxigeno` |
| `0x01` | `char` (1 B) | `b` | `temperatura` |
| `0x02` | `short` (2 B) | `H` | `codigo_colono` |

Formato completo en Python: `struct.pack("<bbH", oxigeno, temperatura, id_colono)`.

---

## 4. Requisitos previos e Instalación

### Requisitos

| Componente | Versión / notas |
| :--- | :--- |
| **GCC** o compatible (Clang) | Obligatorio: el código usa extensiones de GCC (`packed`). **MSVC no está soportado.** |
| **Python** | 3.6 o superior (solo para `transmitter.py`; no requiere `pip install`) |
| **Git** | Para clonar el repositorio |
| **MinGW** | Solo en Windows (aporta `gcc` y `libws2_32`) |

### Instalación en Linux / macOS

```bash
# 1. Clonar el repositorio
git clone https://github.com/victorzurd/The-Spacesuit-Black-Box-Decrypter.git
cd The-Spacesuit-Black-Box-Decrypter

# 2. Compilar los dos ejecutables (recomendado: activar warnings)
gcc -Wall -Wextra -O2 decrypter.c -o decrypter
gcc -Wall -Wextra -O2 satellite_server.c -o satellite_server

# 3. Verificar Python
python3 --version
```

### Instalación en Windows (MinGW)

```powershell
git clone https://github.com/victorzurd/The-Spacesuit-Black-Box-Decrypter.git
cd The-Spacesuit-Black-Box-Decrypter

gcc decrypter.c -o decrypter.exe
gcc satellite_server.c -o satellite_server.exe -lws2_32

# Opcional: habilitar UTF-8 en consola para visualizar correctamente tildes y emojis
chcp 65001
```

> **Nota:** `-lws2_32` es imprescindible en Windows para enlazar Winsock. El repositorio incluye `.exe` precompilados (x86) que pueden ejecutarse directamente.

---

## 5. Variables de entorno

**No aplica.** El proyecto no lee variables de entorno ni ficheros de configuración. Los parámetros están definidos como constantes en el código fuente y se modifican editando y recompilando:

| Constante | Fichero | Valor por defecto | Descripción |
| :--- | :--- | :--- | :--- |
| `PUERTO` | `satellite_server.c` | `8080` | Puerto TCP de escucha del servidor. |
| `PUERTO` | `transmitter.py` | `8080` | Puerto TCP al que se conecta el cliente (debe coincidir con el servidor). |
| `SATELLITE_IP` | `transmitter.py` | `127.0.0.1` | Dirección IP del servidor. |
| `oxigeno`, `temperatura`, `id_colono` | `transmitter.py` | `92`, `19`, `10001` | Valores de telemetría simulada a transmitir. |
| Umbrales de alerta | `satellite_server.c` | O₂ `< 20`, Temp `> 40` | Condición de `ALERTA CRÍTICA`. |

---

## 6. Ejemplo de uso / Protocolo de red

Este proyecto **no expone una API HTTP**: la comunicación es un protocolo TCP binario propio, de un solo sentido (cliente → servidor) y sin respuesta.

### 6.1. Terminal de ingeniería (`decrypter`)

```bash
./decrypter
```

```text
=== TERMINAL DE INGENIERÍA - MISIÓN ARES V ===
0. Salir del Terminal
1. Ver Telemetría Actual del Traje
2. Protocolo de Emergencia: Recargar Oxígeno al 100%
3. Protocolo de Emergencia: Actualizar Temperatura a 25 Grados Celsius
Seleccione una opción: 1

--- TELEMETRÍA RECUPERADA ---
ID del Colono   : #99999
Nivel de Oxígeno: 85%
Temperatura     : 22 Grados Celsius
-----------------------------
```

| Opción | Acción | Operación sobre el fichero |
| :--- | :--- | :--- |
| `0` | Salir | — |
| `1` | Mostrar telemetría | `fread` de 6 bytes |
| `2` | Recargar O₂ al 100 % | `fseek(0)` + `fwrite` de 1 byte |
| `3` | Fijar temperatura a 25 °C | `fseek(1)` + `fwrite` de 1 byte |

Cualquier otra opción cierra el terminal.

### 6.2. Servidor y transmisor por TCP

Abre **dos terminales** en la carpeta del proyecto.

**Terminal 1: iniciar el servidor (satélite)**

```bash
./satellite_server
```

**Terminal 2: enviar telemetría (antena de la colonia)**

```bash
python3 transmitter.py
```

**Salida del servidor:**

```text
[SATÉLITE]: En órbita. Escuchando señales en el puerto 8080...
[SATÉLITE]: ¡Conexión establecida con una antena de la colonia!
[SATÉLITE]: Condiciones de vida estables para el colono #10001

=== TELEMETRÍA INTERPLANETARIA RECIBIDA VIA TCP ===
ID del Colono   : #10001
Nivel de Oxígeno: 92%
Temperatura     : 19 Grados Celsius
===================================================
```

Para provocar una alerta, edita `oxigeno = 15` en `transmitter.py` y vuelve a ejecutarlo:

```text
[SATÉLITE]: ALERTA CRÍTICA: Condiciones de vida comprometidas para el colono #10001
```

### 6.3. Especificación del protocolo

| Propiedad | Valor |
| :--- | :--- |
| Transporte | TCP / IPv4 |
| Puerto | `8080` (escucha en todas las interfaces, `INADDR_ANY`) |
| Conexión | Una trama por conexión; el servidor cierra el socket tras leerla |
| Tamaño de trama | 4 bytes (`<bbH`) |
| Respuesta del servidor | Ninguna |

Cualquier cliente que respete la trama es compatible. Ejemplo sin usar `transmitter.py`:

```bash
python3 -c "import socket,struct; s=socket.create_connection(('127.0.0.1',8080)); s.sendall(struct.pack('<bbH',92,19,10001)); s.close()"
```

---

## 7. Limitaciones conocidas

Aspectos detectados durante el análisis y comprobados al compilar y ejecutar el código:

- **Formatos de registro y de trama inconsistentes**: `decrypter.c` usa `int` (4 B) para `codigo_colono` y `satellite_server.c` usa `short` (2 B). Además, el servidor lo interpreta con signo mientras el emisor lo empaqueta sin signo (`H`), de modo que IDs superiores a `32767` se muestran negativos (por ejemplo, `40000` → `#-25536`).
- **`telemetria.bin` versionado es obsoleto**: el fichero incluido en el repositorio tiene 8 bytes (layout antiguo con *padding*), incompatible con la estructura actual de 6 bytes; `decrypter` mostraría un ID corrupto. **Solución:** elimínalo (`rm telemetria.bin`) y se regenerará al ejecutar `decrypter`.
- **Sin verificación de integridad**: el contexto del proyecto menciona un *checksum*, pero no está implementado en el código.
- **Servidor iterativo y de un solo hilo**: atiende una conexión cada vez, realiza un único `recv` (sin bucle de lectura para tramas parciales) y no tiene parada limpia; el bucle es infinito y se detiene con `Ctrl+C`.
- **Sin autenticación ni cifrado**: el servidor escucha en `0.0.0.0`. No lo expongas a redes no confiables.
- **Entrada de usuario sin validar**: `scanf` en `decrypter` no comprueba entradas no numéricas.
- **Binarios en el repositorio**: los `.exe` versionados deberían excluirse con `.gitignore` y generarse localmente.
