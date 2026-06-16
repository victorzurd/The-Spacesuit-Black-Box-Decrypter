#include <stdio.h>
#include <stdlib.h>

// Forzamos a GCC a empaquetar la estructura para que ocupe exactamente 6 bytes
struct __attribute__((packed)) TelemetriaTraje {
    char nivel_oxigeno;   // 1 byte
    char temperatura;     // 1 byte
    int codigo_colono;    // 4 bytes
};

int main() {
    FILE *archivo_verificar = fopen("telemetria.bin", "rb");
    if (archivo_verificar == NULL) {
        // Si no existe, lo creamos con los valores de fábrica (85%)
        FILE *archivo_escribir = fopen("telemetria.bin", "wb");
        struct TelemetriaTraje colono_prueba = {85, 22, 99999};
        fwrite(&colono_prueba, sizeof(struct TelemetriaTraje), 1, archivo_escribir);
        fclose(archivo_escribir);
        printf("[SISTEMA]: Archivo de telemetría de fábrica creado.\n");
    } else {
        // Si ya existe, lo cerramos y dejamos los datos que tenga (¡incluido tu 100%!)
        fclose(archivo_verificar);
    } // Lo cerramos para asegurarnos de que los datos se guarden en el disco


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
    int continuar = 1;
    while (continuar) {
        printf("\n=== TERMINAL DE INGENIERÍA - MISIÓN ARES V ===\n");
        printf("0. Salir del Terminal\n");
        printf("1. Ver Telemetría Actual del Traje\n");
        printf("2. Protocolo de Emergencia: Recargar Oxígeno al 100%%\n");
        printf("3. Protocolo de Emergencia: Actualizar Temperatura a 25 Grados Celsius\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            FILE *archivo_leer = fopen("telemetria.bin", "rb");
            if (archivo_leer == NULL) {
                printf("🚨 Error al abrir el archivo para lectura.\n");
                return 1;
            }

            struct TelemetriaTraje datos_leidos;
            
            // Leemos los 6 bytes exactos del archivo
            fread(&datos_leidos, sizeof(struct TelemetriaTraje), 1, archivo_leer);
            fclose(archivo_leer);
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
        }else if (opcion == 0) {
            printf("Cerrando terminal de la misión. ¡Buena suerte, colono!\n");
            continuar = 0;
        }else if(opcion == 3){
            // PROTOCOLO DE HACKEO/MODIFICACIÓN EN CALIENTE
            // Abrimos el archivo en modo lectura/escritura binaria ("rb+") sin borrar lo que ya hay
            FILE *archivo_mod = fopen("telemetria.bin", "rb+");
            
            if (archivo_mod == NULL) {
                printf("🚨 Error: No se pudo acceder al sistema de archivos del traje.\n");
                return 1;
            }

            char temperatura_deseada = 25;

            // Usamos fseek para mover el cursor del archivo al byte EXACTO que queremos cambiar.
            // Como 'temperatura' es la segunda variable del struct, su posición es el desplazamiento 1 (SEEK_SET)
            fseek(archivo_mod, 1, SEEK_SET);

            // Sobrescribimos ese byte con el nuevo valor (25)
            fwrite(&temperatura_deseada, sizeof(char), 1, archivo_mod);
            fclose(archivo_mod);

            printf("\n✅ [SOPORTE VITAL]: Temperatura ajustada con éxito en el archivo binario.\n");
        }
        else {
            printf("Opción no válida. Cerrando terminal de la misión.\n");
            continuar = 0;
        }
}

    return 0;
}