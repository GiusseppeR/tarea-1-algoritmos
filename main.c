#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "arrays/array.h"
#include "random/custom_random.h"
#include "sexton_swinbank/ss_utils.h"
#include "query/query.h"
#include "utils.h"
#include "arrays/matrix.h"

int main(void) {
    Allocator allocator = {my_alloc, my_free, 0};
    splitmix64_init((uint64_t) time(NULL));
    xoshiro256plus_init();

    /*int n = 1024;

    Point *input = array(Point, &allocator);

    for(int i = 0; i < n; i++)
        array_append(input, generateRandomPoint());

    Tree * tree = sexton_swinbank(input);

    printf("Tree height: %i\n", tree->height);
    for(int i = 0; i < array_length(tree->entries); i++){
        printf("Entry %i:\n", i);
        printf("Punto: (%f,%f)\n", tree->entries[i].point.x,tree->entries[i].point.y);
        printf("Radio: %f\n", tree->entries[i].radius);
        printf("\n");
    }

    free_tree(tree);
    array_free(input);*/

    int input_sizes[] = {1024,2048,4096, 8192, 16384, 32768, 65536, 131072, 262144,
                         524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432};

    Point **input = matrix(Point, 16, &allocator);

    printf("Generando arreglos aleatorios de tamagno 2^10, 2^11, ..., 2^25 ...\n");

    for (int i = 0; i < array_length(input); i++){
        for (int j = 0; j < input_sizes[i]; j++){
            matrix_append(input, i, generateRandomPoint());
        }
    }

    printf("Inputs generados.\n\n");
    printf("Generando 100 puntos aleatorios para las queries...\n");

    Point *query_centers = array(Point, &allocator);
    for (int i = 0; i < 100; i++){
        array_append(query_centers, generateRandomPoint());
    }

    printf("Puntos generados.\n\n");

    do{
        printf("Escribe 0 para experimentar con arboles CP y 1 para hacerlo con arboles SS\n");
        int selection;

        scanf("%d",&selection);

        if (selection == 0){
            for (int i = 0; i < 16; i++)
                generate_experiment_cp(input[i], query_centers);
        }
        else if(selection == 1){
            for (int i = 0; i < 7; i++)
                generate_experiment_ss(input[i], query_centers);
        }else{
            continue;
        }

        printf("\n");
        printf("Escribe 0 para limpiar resultados y volver a seleccionar.\n");
        printf("Escribe cualquier otra cosa para salir.\n");

        int clear;
        scanf("%d",&clear);
        if (clear == 0){
            continue;
        }
        break;
    } while (TRUE);

    matrix_free(input);
    return 0;
}