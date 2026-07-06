import socket
import struct
import time

# Configuración de la red interplanetaria
SATELLITE_IP = "127.0.0.1"  # Dirección local (localhost)
PUERTO = 8080

print("🚀 [ANTENA COLONIA]: Apuntando hacia el satélite cuántico...")

# 1. Creamos los datos simulados del traje espacial
oxigeno = 92          # 1 byte (char en C)
temperatura = 19      # 1 byte (char en C)
id_colono = 10001    # 2 bytes (short en C)

# 2.Empaquetar los datos en formato binario puro
# Usamos la librería 'struct' con el formato 'bbH':
# '<' = Little-Endian (formato estándar de procesadores modernos)
# 'b' = signed char (1 byte) -> Oxígeno
# 'b' = signed char (1 byte) -> Temperatura
# 'H' = unsigned short (2 bytes) -> ID Colono
# Total = 6 bytes exactos, clavados con el struct de C.
datos_binarios = struct.pack("<bbH", oxigeno, temperatura, id_colono)

try:
    # 3. Abrir el socket TCP
    cliente_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # 4. Conectarse al servidor de C
    cliente_socket.connect((SATELLITE_IP, PUERTO))
    print("📡 [ANTENA COLONIA]: ¡Enlace cuántico establecido!")
    
    # Pequeño delay de ambientación
    time.sleep(1)
    
    # 5. Lanzar los 6 bytes al espacio
    print(f"⚡ [ANTENA COLONIA]: Transmitiendo telemetría del Colono #{id_colono}...")
    cliente_socket.sendall(datos_binarios)
    
    # 6. Cerrar el canal
    cliente_socket.close()
    print("✅ [ANTENA COLONIA]: Transmisión completada. Desconectando.")

except ConnectionRefusedError:
    print("🚨 [ERROR]: No se pudo conectar. ¿Está el satélite en órbita ejecutándose?")