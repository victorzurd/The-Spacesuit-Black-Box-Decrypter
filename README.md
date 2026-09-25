<div align="center">

# 🚀 Satellite-C

### Caja negra y telemetría de trajes espaciales · Misión Ares-V

Herramientas educativas en **C** y **Python** para leer, modificar y transmitir
telemetría binaria de un traje espacial mediante TCP.

<p>
  <img src="https://img.shields.io/badge/C-GCC%20%2F%20MinGW-00599C?style=for-the-badge&logo=c&logoColor=white" alt="C">
  <img src="https://img.shields.io/badge/Python-3.6%2B-3776AB?style=for-the-badge&logo=python&logoColor=white" alt="Python">
  <img src="https://img.shields.io/badge/Network-TCP%20%2F%20IPv4-0B7285?style=for-the-badge" alt="TCP IPv4">
  <img src="https://img.shields.io/badge/Dependencies-0-2F9E44?style=for-the-badge" alt="Sin dependencias">
</p>

</div>

> [!NOTE]
> Este proyecto es una práctica de programación de bajo nivel: estructuras
> empaquetadas, acceso directo a bytes, E/S binaria y sockets multiplataforma.

## ✨ Qué incluye

| Componente | Propósito |
| --- | --- |
| `decrypter` | Terminal interactiva que inspecciona y modifica `telemetria.bin` por *offset*. |
| `satellite_server` | Servidor TCP que recibe una trama binaria y evalúa las constantes vitales. |
| `transmitter.py` | Simulador de antena que envía telemetría al servidor. |

### Flujo de la misión

```text
┌────────────────────┐       TCP · 4 bytes       ┌────────────────────┐
│  transmitter.py    │ ───────────────────────▶ │ satellite_server    │
│  Antena de colonia │                          │ Satélite · :8080   │
└────────────────────┘                          └────────────────────┘

┌────────────────────┐       lectura / escritura
│  decrypter         │ ───────────────────────▶ telemetria.bin
│  Terminal local    │          binaria
└────────────────────┘
```

## 🚀 Inicio rápido

### 1. Compilar

#### Linux / macOS

```bash
git clone https://github.com/victorzurd/The-Spacesuit-Black-Box-Decrypter.git
cd The-Spacesuit-Black-Box-Decrypter

gcc -Wall -Wextra -O2 decrypter.c -o decrypter
gcc -Wall -Wextra -O2 satellite_server.c -o satellite_server
```

#### Windows · MinGW

```powershell
git clone https://github.com/victorzurd/The-Spacesuit-Black-Box-Decrypter.git
cd The-Spacesuit-Black-Box-Decrypter

gcc -Wall -Wextra -O2 decrypter.c -o decrypter.exe
gcc -Wall -Wextra -O2 satellite_server.c -o satellite_server.exe -lws2_32
```

> En Windows, `-lws2_32` es necesario para enlazar Winsock. También se
> incluyen ejecutables precompilados para x86.

### 2. Ejecutar el servidor

En una primera terminal:

```bash
./satellite_server
```

### 3. Transmitir telemetría

En una segunda terminal:

```bash
python3 transmitter.py
```

En Windows, utiliza `python transmitter.py` si `python3` no está disponible.

### 4. Abrir la caja negra

```bash
./decrypter
```

Si `telemetria.bin` no existe, `decrypter` lo crea con estos valores:

| Oxígeno | Temperatura | ID del colono |
| ---: | ---: | ---: |
| `85 %` | `22 °C` | `99999` |

## 🛰️ Terminal de ingeniería

`decrypter` trabaja sobre el fichero sin reescribir el registro completo:

| Opción | Acción |
| ---: | --- |
| `0` | Salir del terminal |
| `1` | Mostrar la telemetría actual |
| `2` | Recargar oxígeno al `100 %` |
| `3` | Ajustar la temperatura a `25 °C` |

Ejemplo:

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

## 📡 Protocolo TCP

El servidor escucha en `0.0.0.0:8080` y espera una única trama por conexión.
No existe respuesta del servidor: el cliente transmite y cierra el socket.

### Formato de la trama

La trama de red ocupa **4 bytes**, está empaquetada en *little-endian* y se
representa en Python con `struct.pack("<bbH", ...)`.

| Offset | Tamaño | Campo | Tipo |
| ---: | ---: | --- | --- |
| `0x00` | 1 byte | `nivel_oxigeno` | `char` con signo |
| `0x01` | 1 byte | `temperatura` | `char` con signo |
| `0x02` | 2 bytes | `codigo_colono` | `unsigned short` |

El servidor clasifica la telemetría así:

- 🔴 **ALERTA CRÍTICA**: oxígeno `< 20 %` o temperatura `> 40 °C`.
- 🟢 **Condiciones estables**: cualquier otro valor.

### Enviar una trama manualmente

Con el servidor ejecutándose, puedes probarlo sin `transmitter.py`:

```bash
python3 -c "import socket,struct; s=socket.create_connection(('127.0.0.1',8080)); s.sendall(struct.pack('<bbH',92,19,10001)); s.close()"
```

Para forzar una alerta, cambia estos valores en `transmitter.py`:

```python
oxigeno = 15
temperatura = 19
```

## 🧱 Estructura del proyecto

```text
Satellite-C/
├── decrypter.c            # Terminal y acceso binario a la caja negra
├── satellite_server.c     # Servidor TCP de telemetría
├── transmitter.py         # Cliente TCP y simulador de telemetría
├── telemetria.bin         # Registro binario local
├── decrypter.exe          # Binario Windows precompilado
├── satellite_server.exe   # Binario Windows precompilado
├── .vscode/               # Configuración de VS Code
└── README.md
```

### Registro local `telemetria.bin`

El registro de disco ocupa **6 bytes** y utiliza un formato diferente al de la
trama de red:

| Offset | Tamaño | Campo |
| ---: | ---: | --- |
| `0x00` | 1 byte | `nivel_oxigeno` |
| `0x01` | 1 byte | `temperatura` |
| `0x02` | 4 bytes | `codigo_colono` (`int`) |

`decrypter` utiliza `fseek` y `fwrite` para actualizar únicamente los bytes
correspondientes al oxígeno o a la temperatura.

## 🛠️ Requisitos

- **GCC** o Clang compatible con `__attribute__((packed))`.
- **Python 3.6 o superior** para `transmitter.py`.
- **MinGW** en Windows para compilar con GCC y enlazar Winsock.
- **Git** para clonar el repositorio.

No hace falta instalar paquetes de Python: el emisor utiliza únicamente la
biblioteca estándar (`socket`, `struct` y `time`).

## ⚙️ Configuración

Los parámetros están definidos directamente en el código:

| Parámetro | Archivo | Valor |
| --- | --- | ---: |
| Puerto del servidor | `satellite_server.c` | `8080` |
| IP de destino | `transmitter.py` | `127.0.0.1` |
| Puerto de destino | `transmitter.py` | `8080` |
| Telemetría simulada | `transmitter.py` | `92 %`, `19 °C`, `10001` |
| Umbrales críticos | `satellite_server.c` | O₂ `< 20`, temp. `> 40` |

## ⚠️ Limitaciones conocidas

- El registro local usa un `int` de 4 bytes para el ID, mientras que la trama
  de red usa un `short` de 2 bytes.
- El servidor realiza un único `recv` por conexión, por lo que no reensambla
  tramas TCP parciales.
- El servidor es iterativo, no tiene autenticación ni cifrado y escucha en
  todas las interfaces. **No lo expongas a redes no confiables.**
- La entrada numérica de `decrypter` no valida errores de `scanf`.
- El `telemetria.bin` incluido puede proceder de un layout antiguo de 8 bytes.
  Si muestra datos corruptos, elimínalo y deja que `decrypter` lo regenere:

  ```bash
  rm telemetria.bin
  ```

- Los ejecutables `.exe` están incluidos como referencia; para desarrollo se
  recomienda compilarlos localmente.

## 📄 Licencia

Este repositorio no declara actualmente una licencia. Consulta al propietario
antes de redistribuirlo o incorporarlo a otro proyecto.
