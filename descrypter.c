#include <stdio.h>
#include <stdlib.h>

// Definimos la estructura de la caja negra del traje
struct TrajeEspacial {
    char nivel_oxigeno;   // 1 byte
    char temperatura;     // 1 byte
    int codigo_colono;    // 4 bytes
};

int main() {
    // 1. Abrimos el archivo binario
    FILE *archivo = fopen("telemetria.bin", "rb");
    if (archivo == NULL) {
        printf("Error: No se pudo recuperar la caja negra.\n");
        return 1;
    }

    // 2. Creamos una variable con nuestra estructura para guardar los datos
    struct TrajeEspacial datos_traje;

    // 3. Leemos los bytes del archivo y los metemos en la estructura
    // fread(dónde_guardar, tamaño_del_dato, cuántos_datos, archivo)
    fread(&datos_traje, sizeof(struct TrajeEspacial), 1, archivo);

    // 4. Cerramos el archivo (buena práctica de gestión de memoria)
    fclose(archivo);

    // 5. Mostramos los datos descifrados por pantalla
    printf("--- DATOS DE TELEMETRÍA RECUPERADOS ---\n");
    printf("ID del Colono: %d\n", datos_traje.codigo_colono);
    printf("Nivel de Oxígeno: %d%%\n", datos_traje.nivel_oxigeno);
    printf("Temperatura del Traje: %d Grados\n", datos_traje.temperatura);

    return 0;
}