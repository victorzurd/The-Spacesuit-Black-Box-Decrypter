#include <stdio.h>
#include <stdlib.h>

// Forzamos a GCC a empaquetar la estructura para que ocupe exactamente 6 bytes
struct __attribute__((packed)) TelemetriaTraje {
    char nivel_oxigeno;   // 1 byte
    char temperatura;     // 1 byte
    int codigo_colono;    // 4 bytes
};

int main() {
    // =====================================================================
    // PASO 1: GENERAR EL ARCHIVO BINARIO LIMPIO (Simulamos la grabación del traje)
    // =====================================================================
    FILE *archivo_escribir = fopen("telemetria.bin", "wb"); // "wb" sobreescribe y limpia el archivo
    if (archivo_escribir == NULL) {
        printf("🚨 Error al crear el archivo de simulación.\n");
        return 1;
    }

    // Creamos los datos reales de prueba: Oxigeno=85, Temperatura=22, ID=99999
    struct TelemetriaTraje colono_prueba = {85, 22, 99999};
    
    // Escribimos la estructura entera en el archivo
    fwrite(&colono_prueba, sizeof(struct TelemetriaTraje), 1, archivo_escribir);
    fclose(archivo_escribir); // Lo cerramos para asegurarnos de que los datos se guarden en el disco


    // =====================================================================
    // PASO 2: LEER LA CAJA NEGRA (Tu decodificador)
    // =====================================================================
    FILE *archivo_leer = fopen("telemetria.bin", "rb");
    if (archivo_leer == NULL) {
        printf("🚨 Error al abrir el archivo para lectura.\n");
        return 1;
    }

    struct TelemetriaTraje datos_leidos;
    
    // Leemos los 6 bytes exactos del archivo
    fread(&datos_leidos, sizeof(struct TelemetriaTraje), 1, archivo_leer);
    fclose(archivo_leer);


  // =====================================================================
    // PASO 3: MENÚ INTERACTIVO DE LA BASE ESPACIAL
    // =====================================================================
    int opcion;
    printf("\n=== TERMINAL DE INGENIERÍA - MISIÓN ARES V ===\n");
    printf("1. Ver Telemetría Actual del Traje\n");
    printf("2. Protocolo de Emergencia: Recargar Oxígeno al 100%%\n");
    printf("Seleccione una opción: ");
    scanf("%d", &opcion);

    if (opcion == 1) {
        // Mostramos los datos actuales que ya leímos en el Paso 2
        printf("\n--- TELEMETRÍA RECUPERADA ---\n");
        printf("ID del Colono   : #%d\n", datos_leidos.codigo_colono);
        printf("Nivel de Oxígeno: %d%%\n", datos_leidos.nivel_oxigeno);
        printf("Temperatura     : %d Grados Celsius\n", datos_leidos.temperatura);
        printf("-----------------------------\n\n");
    } 
    else if (opcion == 2) {
        // PROTOCOLO DE HACKEO/MODIFICACIÓN EN CALIENTE
        // Abrimos el archivo en modo lectura/escritura binaria ("rb+") sin borrar lo que ya hay
        FILE *archivo_mod = fopen("telemetria.bin", "rb+");
        
        if (archivo_mod == NULL) {
            printf("🚨 Error: No se pudo acceder al sistema de archivos del traje.\n");
            return 1;
        }

        char oxigeno_maximo = 100;

        // Usamos fseek para mover el cursor del archivo al byte EXACTO que queremos cambiar.
        // Como 'nivel_oxigeno' es la primera variable del struct, su posición es el desplazamiento 0 (SEEK_SET)
        fseek(archivo_mod, 0, SEEK_SET);

        // Sobrescribimos ese byte con el nuevo valor (100)
        fwrite(&oxigeno_maximo, sizeof(char), 1, archivo_mod);
        fclose(archivo_mod);

        printf("\n✅ [SOPORTE VITAL]: Oxígeno inyectado con éxito en el archivo binario.\n");
        printf("Reejecuta el programa y selecciona la Opción 1 para verificar los nuevos niveles.\n\n");
    } 
    else {
        printf("Opción no válida. Cerrando terminal de la misión.\n");
    }

    return 0;
}