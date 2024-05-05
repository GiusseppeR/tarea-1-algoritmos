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
    Point medoide={0,0};
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
    Tree C = {0, 0, (Entry*)array(Entry,&my_allocator),(Entry*)array(Entry,&my_allocator)};
    for(int i = 0; i < n; i++){ //para cada punto del input
        Entry p = {input[i], 0, NULL}; //aramamos un entry
        array_append(C.entries, p); //lo agregamos a C
        C.size++;
        double r_a = squaredDistance(g,p.point); //distancia
        r = ( r > r_a) ? r : r_a;//recalculamos r
    }
    Entry ret = {g, r, &C }; //creamos entry de hoja
    array_append(C.parent,ret);
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

Point find_closest_to(Point * points, Point pivot) {
    int min =-1;
    Point closest = {0,0};
    for(int i=0; i < array_length(points); i++) {
        if(squaredDistance(points[i], pivot) < min) {
            min = squaredDistance(points[i], pivot);
            closest= points[i];
        }
    }
    return closest;
}

//funcion para buscar el vecino mas cercano a c en cout
Cluster cluster_nearest_neighbor(Cluster * Cout, Cluster c, Point * Cout_medoids) {
    Cluster ret;
    int find_closest_index= c.index_primary_medoide;
    Point c_medoid= c.array[c.index_primary_medoide]; //guardamos su medoide
    //ahora en los medoides de Cout q son los guardados en C_prima queremos encontrar el cluster mas cercano a c
    Point closest_medoid = find_closest_to(Cout_medoids,c_medoid); //buscamos el mas cercano en cout al medoide de c


    return ret;
}

Point * get_clusters_points(Cluster c1, Cluster c2) {
    Point * all_points = array(Point,&my_allocator);
    int c1_lenght = array_length(c1.array);
    int c2_lenght = array_length(c2.array);
    int n = (c1_lenght>c2_lenght) ? c1_lenght : c2_lenght;
    for (int i = 0; i<n; i++) {
        if (i<c1_lenght) {
            array_append(all_points,c1.array[0]);
        }
        if (i<c2_lenght) {
            array_append(all_points,c2.array[0]);
        }
    }
    return all_points;
}

Cluster *min_max_policy(Cluster c1, Cluster c2) {
    Cluster result[2];
    Point * all_points = get_clusters_points(c1,c2);
    Point random_points[2];
    for (int i = 0; i<array_length(all_points)-1; i++) {
        int j = i+1;
        while (j < array_length(all_points)) {
            random_points[0] = all_points[i];
            random_points[1] = all_points[j];
            j++;
        }
    }

}

//Metodo para Cluster
static Cluster * cluster(Point* input) { // ver si nos dan "n"
    Cluster *C = array(Cluster, &my_allocator); //array de puntos c
    Cluster *Cout = array(Cluster, &my_allocator); //array de puntos cout
    Point *C_prima = array(Point, &my_allocator);
    Point *Cout_medoids = array(Point, &my_allocator);
    for(int i=0; i < array_length(input); i++) {
        array_append(C_prima, input[i]); //en c prima guardaremos los medoides de cada cluster
        Point *subset = array(Point, &my_allocator); // creamos un sub conjunto de puntos {}
        array_append(subset, input[i]); // se agrega el input[i]
        Cluster sub_cluster = {subset, 0}; // cremos el cluster ({p},0)
        array_append(C, sub_cluster); //se añaden los subconjutos con cada punto del input a C
    }//C = {({p1}, 0), ({p2}, 0), ({p3}, 0) ...}
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
            array_append(Cout_medoids, c1.array[c1.index_primary_medoide]);
            array_append(Cout,c1); //y lo agregamos solito a c
        }
    }
    Cluster c = C[0]; //se define c como el unico elemento que queda en c
    Cluster *c_prima = NULL; //iniciamos c prima nulo
    if (array_length(Cout) >0) { //si el tamaño de cout es mayor a 0
        *c_prima = cluster_nearest_neighbor(Cout,c, Cout_medoids); //buscamos en cout el vecino mas cercano a c
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
/**
 *  @brief  Función que retorna todos los puntos de un arreglo de entries
 */
Point * entries_get_points(Entry * entries) {
    Point * points = array(Point, &my_allocator);
    for (int i = 0; i<array_length(entries); i++) {
        array_append(points,entries[i].point);
    }
    return points;
}

/**
 * @brief Función que retorna un entry (G,R,A)
 */
static Entry internal(Tree* c_mra) {
    Point * c_in = entries_get_points(c_mra->entries); // Agrupo solo los puntos de las entries
    Point g = primaryMedoide(c_in); // Encuentro el medoide primario de este
    double r = 0.0; // Seteo el radio en 0
    for (int i = 0; i<array_length(c_mra); i++) {
        r = fmax(r,squaredDistance(g,c_in[i]) + c_mra->entries[i].radius); // Voy actualizando la R con cada punto de c_in
    }
    Entry result = {g,r,c_mra}; // Creo el entry de salida
    Entry * c = array(Entry,&my_allocator); // creo el arreglo final
    array_append(c,result); // añado el resultado al arreglo c, que va a corresponder al padre del nodo
    c_mra->parent = c; // seteo c como el padre del arreglo de entries c_mra
    return result;
}

Entry * entry_filter(Cluster c, Entry *p) {
    Entry * final = array(Entry, &my_allocator);

}

static Tree * sexton_swinbank(Point * c_in) {
    /*
     * Caso base
     */
    if (array_length(c_in)<=B) {
        Tree * final_tree = array(Tree,&my_allocator);
        const Entry final = leaf(c_in);
        array_append(final_tree,*final.subTree);
        return final_tree;
    }
    Cluster * c_out = cluster(c_in);
    Entry * c_entries = array(Entry, &my_allocator);
    for (int i = 0; i<array_length(c_out); i++) {
        array_append(c_entries,leaf(c_out[i].array));
    }
    Tree c = {1,array_length(c_entries),c_entries,NULL};
    while (c.size > B) {
        c_in = entries_get_points(c.entries);
        c_out = cluster(c_in);
        Entry ** c_mra = array(Entry *,&my_allocator);
        for (int i=0; i<array_length(c_out);i++) {
            Entry * s = entry_filter(c_out[i],c.entries);
            if (s != NULL)
                array_append(c_mra,s);
        }
        c.size = 0;
        array_free(c.entries);
        c.entries = array(Entry,&my_allocator);
        for (int i = 0; i<array_length(c_mra);i++) {
            array_append(c.entries,internal(c_mra[i]));
        }
        c.size = array_length(c.entries);
        c.height+=1;
    }
    const Entry final_entry = internal(&c);
    Entry *final_entry_ptr = malloc(sizeof(Entry));
    *final_entry_ptr = final_entry;
    return final_entry_ptr->subTree;
}