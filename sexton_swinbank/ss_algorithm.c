#include <math.h>
#include <stdlib.h>
#include "../structures.h"
#include "../utils.h"
#include "../arrays/array.h"
#include "../arrays/matrix.h"
#include "../trees/2dTrees.h"
#include "../random/custom_random.h"

Allocator my_allocator = {my_alloc, my_free, 0};


/**
 * @brief función que calcula el punto centroide de un set de puntos
 *
 * @param input arreglo de puntos
 * @return el centroide del arreglo
 */
static Point centroid(Point * input) {
    long int const n = array_length(input); //ver si n es dado.
    double sum_x = 0.0;
    double sum_y = 0.0;

    // Sumar las coordenadas x e y de todos los puntos
    for (int i = 0; i < n; i++) {
        sum_x += input[i].x;
        sum_y += input[i].y;
    }

    // Calcular las coordenadas del centroide
    Point centroid_point; // index no está inicializado.
    centroid_point.x = sum_x / n;
    centroid_point.y = sum_y / n;

    return centroid_point;
}
//funcion para definir medoide primario
static Point primaryMedoide(Point * input) {
    if (array_length(input) <= 2) { //caso de un punto o 2 puntos se retorna el primero como medoide
        return input[0];
    }
    //para calcular el medoide calcularemos el centroide
    Point c = centroid(input);
    Point medoide;
    double max= INFINITY;
    //y luego veremos el punto más cercano a este
    for(int i=0; i < array_length(input); i++) {
        double pivot= squaredDistance(c, input[i]);
        if(pivot < max) {
            max= pivot;
            medoide = input[i];
        }
    }
    return medoide;

}

//Metodo para output hoja
static Tuple leaf(Point * input){
    Point g = primaryMedoide(input); //medoide primario de input
    double r = 0; //radio
    int *C = array(Tuple, &my_allocator); //Nodo hoja
    for(int i = 0; i < array_length(input); i++){ //para cada punto del input
        Tuple p = {input[i], NULL, NULL}; //aramamos una tupla
        array_append(C, &p); //lo agregamos a C
        double r_a = squaredDistance(g,p.g); //distancia
        r = ( r > r_a) ? r : r_a;//recalculamos r
    }
    Tuple ret = {g, &r, &C }; //creamos tupla de hoja
    return ret; //se retorna
}

static Cluster * find_nearest_cluster(Cluster *C) {

}
//meter al utils
int compare(Point p1, Point p2) {
    return (p1.x == p2.x) & (p1.y == p2.y);
}

Cluster cluster_union(Cluster c1, Cluster c2) {
    Cluster cluster_res;
}

void cluster_remove(Cluster c1, Cluster *c2) {
}
Cluster cluster_nearest_neighbor(Cluster * Cout, Cluster c) {
    Cluster pivote;
    return pivote;
}

Cluster *min_max_policy(Cluster c1, Cluster c2) {

}

//Metodo para Cluster
static Cluster * cluster(Point* input) { // ver si nos dan "n"
    Cluster *C = array(Cluster, &my_allocator); //array de puntos c
    Cluster *Cout = array(Cluster, &my_allocator); //array de puntos cout
    Point *C_prima = array(Point, &my_allocator);
    long int n = array_length(input);
    for(int i=0; i < n; i++) {
        array_append(C_prima, input[i]);
        Point *subset = array(Point, &my_allocator); // creamos un sub conjunto de puntos {}
        array_append(subset, input[i]); // se agrega el input[i]
        Tuple tuple = {input[i], NULL, NULL}; // creamos la tupla para input[i]
        Cluster sub_cluster = {subset, &tuple}; // cremos el cluster
        array_append(C, sub_cluster); //se añaden los subconjutos con cada punto del input a C
    }//C = {{p1}, {p2}, {p3} ...}
    while(array_length(C) > 1) {
        ClosestPoints closest_points = closest(C_prima,array_length(C_prima));
        Cluster cluster1,cluster2;
        for (int i = 0; i<array_length(C); i++) {
            if (compare(C[i].tuple->g,closest_points.point1)) {
                cluster1 = C[i];
            }
            else if (compare(C[i].tuple->g,closest_points.point2)){
                cluster2 = C[i];
            }
        }
        Cluster c1 = (array_length(cluster1.array) > array_length(cluster2.array))? cluster1: cluster2;
        Cluster c2 = (array_length(cluster1.array) <= array_length(cluster2.array))? cluster1: cluster2;
        if (array_length(c1.array) + array_length(c2.array) <=B) {
            cluster_remove(c1,C);
            cluster_remove(c2,C);
            Cluster new_cluster = cluster_union(c1,c2);
            array_append(C,new_cluster);
        }
        else {
            cluster_remove(c1,C);
            array_append(Cout,c1);
        }
    }
    Cluster c = C[0];
    Cluster *c_prima = NULL;
    if (array_length(Cout) >0) {
        *c_prima = cluster_nearest_neighbor(Cout,c); //diagnosticar
        cluster_remove(*c_prima,Cout);
    }
    if ((array_length(c.array) + array_length(c_prima->array))<=B) {
        Cluster new_cluster = cluster_union(c,*c_prima);
        array_append(Cout,new_cluster);
    }
    else {
        Cluster *final_clusters = min_max_policy(c,*c_prima);
        Cluster c1 = final_clusters[0], c2 = final_clusters[1];
        array_append(Cout,c1);
        array_append(Cout,c2);
    }
    return Cout;
}




