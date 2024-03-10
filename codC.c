#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void iniciar(void);

// Estructura para almacenar los datos de cada estudiante
typedef struct Estudiante {
    char nombre[50];
    char apellido1[50];
    char apellido2[50];
    int nota;
    struct Estudiante *siguiente;
} Estudiante;

// Estructura para almacenar la configuración
typedef struct {
    int nota_aprobacion;
    int nota_reposicion;
    int tamano_grupos;
    int escala_grafico;
    char ordenamiento[100];
} Configuracion;

// Función para comparar estudiantes por nombre (para qsort)
int comparar_estudiantes_alfabetico(const void *a, const void *b) {
    Estudiante *estudianteA = *(Estudiante**)a;
    Estudiante *estudianteB = *(Estudiante**)b;
    return strcmp(estudianteA->nombre, estudianteB->nombre);
}

// Función para comparar estudiantes por nota (para qsort)
int comparar_estudiantes_numerico(const void *a, const void *b) {
    Estudiante *estudianteA = *(Estudiante**)a;
    Estudiante *estudianteB = *(Estudiante**)b;
    return estudianteB->nota - estudianteA->nota;  // Orden descendente
}

// Función para leer el archivo de configuración
void leer_configuracion(char *archivo, Configuracion *config) {
    FILE *file = fopen(archivo, "r");
    char linea[100];

    if (file == NULL) {
        printf("Error al abrir el archivo de configuración.\n");
        return;
    }

    while (fgets(linea, sizeof(linea), file)) {
        // Copiamos la línea a una nueva cadena y eliminamos el carácter de nueva línea
        char linea_sin_salto[100];
        strncpy(linea_sin_salto, linea, sizeof(linea_sin_salto) - 1);
        linea_sin_salto[sizeof(linea_sin_salto) - 1] = '\0';
        char *ptr = strchr(linea_sin_salto, '\n');
        if (ptr != NULL) {
            *ptr = '\0';
        }

        // Imprimimos la línea antes de procesarla
        printf("Línea leída antes: %s\n", linea_sin_salto);

        if (strstr(linea_sin_salto, "Nota de aprobación:") != NULL) {
            sscanf(linea_sin_salto, "Nota de aprobación: %d", &config->nota_aprobacion);
            printf(" Valor de nota_aprobacion: %d\n", config->nota_aprobacion);
        } else if (strstr(linea_sin_salto, "Nota de Reposisción:") != NULL) {
            sscanf(linea_sin_salto, "Nota de Reposisción: %d", &config->nota_reposicion);
            printf(" Valor de nota_reposicion: %d\n", config->nota_reposicion);
        } else if (strstr(linea_sin_salto, "Tamaño de los grupos de notas:") != NULL) {
            sscanf(linea_sin_salto, "Tamaño de los grupos de notas: %d", &config->tamano_grupos);
            printf(" Valor de tamano_grupos: %d\n", config->tamano_grupos);
        } else if (strstr(linea_sin_salto, "Escala del gráfico:") != NULL) {
            sscanf(linea_sin_salto, "Escala del gráfico: %d", &config->escala_grafico);
            printf(" Valor de escala_grafico: %d\n", config->escala_grafico);
        } else if (strstr(linea_sin_salto, "Ordenamiento:") != NULL) {
            sscanf(linea_sin_salto, "Ordenamiento: %s", config->ordenamiento);
            printf(" Valor de ordenamiento: %s\n", config->ordenamiento);
            break;  // Termina el bucle después de leer "Ordenamiento"
        }
    }

    fclose(file);
}


// Función para leer el archivo de datos
void leer_datos(char *archivo, Configuracion *config, Estudiante **estudiantes, int *num_estudiantes) {
    FILE *file = fopen(archivo, "r");
    char linea[100];
    Estudiante *estudiante;

    if (file == NULL) {
        printf("Error al abrir el archivo de datos.\n");
        return;
    }

    while (fgets(linea, sizeof(linea), file)) {
        estudiante = malloc(sizeof(Estudiante));
        sscanf(linea, "%s %s %s: %d", estudiante->nombre, estudiante->apellido1, estudiante->apellido2, &estudiante->nota);
        estudiante->siguiente = *estudiantes;
        *estudiantes = estudiante;
        (*num_estudiantes)++;
    }

    fclose(file);

    // Ordena los estudiantes si la configuración lo especifica
    if (strcmp(config->ordenamiento, "alfabético") == 0) {
        *estudiantes = realloc(*estudiantes, *num_estudiantes * sizeof(Estudiante*));
        qsort(*estudiantes, *num_estudiantes, sizeof(Estudiante*), comparar_estudiantes_alfabetico);
    } else if (strcmp(config->ordenamiento, "numérico") == 0) {
        *estudiantes = realloc(*estudiantes, *num_estudiantes * sizeof(Estudiante*));
        qsort(*estudiantes, *num_estudiantes, sizeof(Estudiante*), comparar_estudiantes_numerico);
    }
}

// Función para escribir los datos ordenados en un archivo
void escribir_datos(char *archivo, Estudiante *estudiantes, int num_estudiantes) {
    FILE *file = fopen(archivo, "w");
    int i;

    if (file == NULL) {
        printf("Error al abrir el archivo de salida.\n");
        return;
    }

    for (i = 0; i < num_estudiantes; i++) {
        fprintf(file, "%s %s %s: %d\n", estudiantes[i].nombre, estudiantes[i].apellido1, estudiantes[i].apellido2, estudiantes[i].nota);
    }

    fclose(file);
}

void liberar_estudiantes(Estudiante *estudiantes) {
    Estudiante *actual = estudiantes;
    Estudiante *siguiente;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}


int main() {
    Configuracion config;
    Estudiante *estudiantes = NULL;
    int num_estudiantes = 0;

    // Lee la configuración primero
    leer_configuracion("configuracion.txt", &config);

    // Luego, inicia el programa
    iniciar();

    // Después, lee los datos de los estudiantes
    leer_datos("datos.txt", &config, &estudiantes, &num_estudiantes);

    // Realiza otras operaciones según sea necesario

    // Finalmente, escribe los datos ordenados
    escribir_datos("datos_ordenados.txt", estudiantes, num_estudiantes);

    // Libera la memoria antes de finalizar el programa
    liberar_estudiantes(estudiantes);

    return 0;
}

