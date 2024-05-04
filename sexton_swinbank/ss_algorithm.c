#include <math.h>
#include <stdlib.h>
#include "../structures.h"
#include "../utils.h"
#include "../arrays/array.h"
#include "../arrays/matrix.h"
#include "../trees/2dTrees.h"
#include "../random/custom_random.h"

Allocator my_allocator = {my_alloc, my_free, 0};

//funcion para definir medoide primario
static Point primaryMedoide(Point * input) {
    double min =-1; //iniciamos un min
    Point medoide;
    for(int i=0; i < array_length(input); i++) {
        double dist=0; //iniciamos la distancia para actualizarla
        for(int k=0; k < array_length(input); i++ ) {
            dist+= squaredDistance(input[i], input[k]); //agregamos la distancia
        }
        if(dist < min){ //si encontramos un nuevo min lo actualizamos y actualizamos el medoide
            min=dist;
            medoide= input[i];
        }
    }
    return medoide;
}

//Metodo para output hoja
static Entry leaf(Point * input){
    int n = array_length(input);
    Point g = primaryMedoide(input); //medoide primario de input
    double r = 0; //radio
    Tree C = {0, 0, (Entry*)array(Entry,&my_allocator),NULL};
    for(int i = 0; i < n; i++){ //para cada punto del input
        Entry p = {input[i], 0, NULL}; //aramamos una tupla
        array_append(C.entries, p); //lo agregamos a C
        C.size++;
        double r_a = squaredDistance(g,p.point); //distancia
        r = ( r > r_a) ? r : r_a;//recalculamos r
    }
    Entry ret = {g, r, &C }; //creamos tupla de hoja
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
        array_append(C_prima, input[i]); //en c prima guardaremos los medoides de cada cluster
        Point *subset = array(Point, &my_allocator); // creamos un sub conjunto de puntos {}
        array_append(subset, input[i]); // se agrega el input[i]
        Cluster sub_cluster = {subset, i}; // cremos el cluster {[]}
        array_append(C, sub_cluster); //se añaden los subconjutos con cada punto del input a C
    }//C = {{p1}, {p2}, {p3} ...}
    while(array_length(C) > 1) { //mientras el tamaño de C sea mayor a 1 es decir mientras podemos separar
        ClosestPoints closest_points = closest(C_prima,array_length(C_prima));  //primero buscamos el par de medoides mas cercanos en cprima
        Cluster cluster1,cluster2; //luego buscamos a que cluster corresponden
        for (int i = 0; i<array_length(C); i++) {
            if (compare(*C[i].array,closest_points.point1)) { //funciona?? accede a valor del punto
                cluster1 = C[i];
            }
            else if (compare(*C[i].array,closest_points.point2)){
                cluster2 = C[i];
            }
        }
        Cluster c1 = (array_length(cluster1.array) > array_length(cluster2.array))? cluster1: cluster2; //guardamos en c1 el cluster de mayor tamaño
        Cluster c2 = (array_length(cluster1.array) <= array_length(cluster2.array))? cluster1: cluster2;
        if (array_length(c1.array) + array_length(c2.array) <=B) { //si la suma de tamaño de arreglos es menor a lo q cabe en un bloque
            cluster_remove(c1,C); //sacamos los clusters de c
            cluster_remove(c2,C);
            Cluster new_cluster = cluster_union(c1,c2); //los unimos
            array_append(C,new_cluster); //y los añadimos unidos a c
        }
        else {
            cluster_remove(c1,C); //si no quitamos el mas grande
            array_append(Cout,c1); //y lo agregamos solito a c
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




