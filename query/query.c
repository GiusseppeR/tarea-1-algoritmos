#include "../structures.h"
#include "../utils.h"
#include "time.h"
#include <math.h>
#include "../arrays/matrix.h"

Allocator alloc = {my_alloc, my_free, 0};
void query(const Tree tree, const Point point, const double radius, Point ** result, int * accesses){
    double sqr_radius = radius;
    *accesses += 1;
    for (int i = 0; i < tree.size; i++){
        Entry entry = tree.entries[i];
        double distance = sqrt(squaredDistance(point, entry.point));
        if(distance > sqr_radius + entry.radius)
            continue;

        if(entry.subTree == NULL)
            array_append(*result,entry.point);
        else
            query(*entry.subTree, point, radius, result, accesses);
    }
}

//cambiarle la firma a hundred_queries(Tree tree_cp, Tree tree_ss, int n)
static void hundred_queries(Tree tree,int n, Point * input){
    int mean_accesses = 0;
    int mean_points = 0;
    double mean_percentage = 0;
    int * values = array(int, &alloc);
    for(int i = 0; i < array_length(input); i++){
        Point * results = array(Point, &alloc);
        int accesses = 0;
        Point center = input[i];
        query(tree, center, 0.02, &results, &accesses);
        array_append(values, accesses);
        mean_accesses += accesses;
        mean_points += array_length(results);
        mean_percentage += (double)array_length(results) / (double)n * 100;

        array_free(results);
    }
    double mean = (double) mean_accesses/100;
    double standar_deviation = 0;
    for (int i = 0; i < array_length(values); i++){
        standar_deviation += (values[i] - mean) * (values[i] - mean);
    }
    standar_deviation = sqrt(standar_deviation/ (double)(array_length(values) - 1));

    double confianza1 = mean - 1.96*standar_deviation/sqrt(array_length(values));
    double confianza2 = mean + 1.96*standar_deviation/sqrt(array_length(values));

    printf("Cantidad de accessos promedio: %i\n", mean_accesses/100);
    printf("Intervalo de confianza: [%f, %f]\n", confianza1, confianza2);
    printf("Cantidad de puntos promedio: %i\n", mean_points/100);
    printf("Porcentaje de cobertura promedio: %f\n", mean_percentage/100);
}

void generate_experiment_cp(Point * input, Point* query_points) {
    printf("##################################################\n");
    printf("Utilizando %zu puntos\n", array_length(input));
    struct timespec t1, t2;

    clock_gettime(CLOCK_MONOTONIC,&t1);

    Tree * tree_cp = ciaccia_patella(input);

    printf("Arbol de tamanno %i construido con Ciaccia-Patella.\n", tree_cp->height);

    clock_gettime(CLOCK_MONOTONIC,&t2);

    double delta_t;
    delta_t = (t2.tv_sec - t1.tv_sec) * 1e9;
    delta_t = (delta_t + (t2.tv_nsec - t1.tv_nsec)) * 1e-9;

    printf("Tiempo de construccion: %f segundos.\n", delta_t);
    printf("\n");

    printf("Resultados de 100 queries:\n");

    hundred_queries(*tree_cp, array_length(input), query_points);

    free_tree(tree_cp);
    printf("##################################################\n");
    printf("\n");
}

void generate_experiment_ss(Point * input, Point* query_points) {
    printf("##################################################\n");
    printf("Utilizando %zu puntos\n", array_length(input));
    struct timespec t1, t2;

    clock_gettime(CLOCK_MONOTONIC,&t1);


    Tree * tree_ss = sexton_swinbank(input);

    printf("Arbol de tamanno %i construido con Sexton-Swinbank.\n", tree_ss->height);

    clock_gettime(CLOCK_MONOTONIC,&t2);

    double delta_t;
    delta_t = (t2.tv_sec - t1.tv_sec) * 1e9;
    delta_t = (delta_t + (t2.tv_nsec - t1.tv_nsec)) * 1e-9;

    printf("Tiempo de construccion: %f segundos. \n", delta_t);
    printf("\n");

    printf("Resultados de 100 queries:\n");

    hundred_queries(*tree_ss, array_length(input), query_points);

    free_tree(tree_ss);
    printf("##################################################\n");
    printf("\n");
}
