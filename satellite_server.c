#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h> 
#endif

#define PUERTO 8080

struct __attribute__((packed)) TelemetriaTraje {
    char nivel_oxigeno;
    char temperatura;
    short codigo_colono;
};

int main() {
    int servidor_fd, nuevo_socket;
    struct sockaddr_in direccion;
    int opt = 1;
    int addrlen = sizeof(direccion);
    struct TelemetriaTraje datos_recibidos;

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        fprintf(stderr, "Error iniciando Winsock\n");
        return 1;
    }
#endif

    printf("📡 [SATÉLITE]: Iniciando sistemas de comunicación cuántica...\n");

    if ((servidor_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    
    setsockopt(servidor_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY; 
    direccion.sin_port = htons(PUERTO);     

    if (bind(servidor_fd, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) {
        perror("Error en el Bind (Puerto ocupado)");
        exit(EXIT_FAILURE);
    }

    if (listen(servidor_fd, 3) < 0) {
        perror("Error en Listen");
        exit(EXIT_FAILURE);
    }

    printf("[SATÉLITE]: En órbita. Escuchando señales en el puerto %d...\n", PUERTO);
    while (1) {
        if ((nuevo_socket = accept(servidor_fd, (struct sockaddr *)&direccion, (socklen_t*)&addrlen)) < 0) {
            perror("Error en Accept");
            exit(EXIT_FAILURE);
        }

        printf("[SATÉLITE]: ¡Conexión establecida con una antena de la colonia!\n");

        
        int bytes_leidos = recv(nuevo_socket, (char*)&datos_recibidos, sizeof(struct TelemetriaTraje), 0);

        if (bytes_leidos > 0) {
            if(datos_recibidos.nivel_oxigeno < 20 || datos_recibidos.temperatura > 40) {
                printf("[SATÉLITE]: ALERTA CRÍTICA: Condiciones de vida comprometidas para el colono #%d\n", datos_recibidos.codigo_colono);
            } else {
                printf("[SATÉLITE]: Condiciones de vida estables para el colono #%d\n", datos_recibidos.codigo_colono);
            }
            printf("\n=== TELEMETRÍA INTERPLANETARIA RECIBIDA VIA TCP ===\n");
            printf("ID del Colono   : #%d\n", datos_recibidos.codigo_colono);
            printf("Nivel de Oxígeno: %d%%\n", datos_recibidos.nivel_oxigeno);
            printf("Temperatura     : %d Grados Celsius\n", datos_recibidos.temperatura);
            printf("===================================================\n\n");
        }
    

        #ifdef _WIN32
            closesocket(nuevo_socket);
        #else
            close(nuevo_socket);
        #endif
        
    } 
    #ifdef _WIN32
        closesocket(servidor_fd);
        WSACleanup();
    #else
        close(servidor_fd);
    #endif
    
    printf("📡 [SATÉLITE]: Transmisión finalizada. Cerrando canal.\n");
    return 0;
}