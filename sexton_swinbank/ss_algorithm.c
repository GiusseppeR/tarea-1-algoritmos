#include <math.h>
#include <stdlib.h>
#include "../structures.h"
#include "../utils.h"
#include "../arrays/array.h"
#include "../arrays/matrix.h"
#include "../trees/2dTrees.h"
#include "../random/custom_random.h"

Allocator my_other_allocator = {my_alloc, my_free, 0};
void show_cluster(Cluster c);
static Point * get_clusters_points(Cluster c1, Cluster c2);
//funcion para definir medoide primario
static Point primary_medoide(Point * input) {
    double min =INFINITY; //iniciamos un min
    Point medoide={0,0};
    for(int i=0; i < array_length(input); i++) {
        double dist=0; //iniciamos la distancia para actualizarla
        for(int k=0; k < array_length(input); k++ ) {
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
    Point g = primary_medoide(input); //medoide primario de input
    double r = 0; //radio
    Tree C = {1, 0, (Entry*)array(Entry,&my_other_allocator),NULL};
    for(int i = 0; i < array_length(input); i++){ //para cada punto del input
        Entry p = {input[i], 0, NULL}; //aramamos un entry
        array_append(C.entries, p); //lo agregamos a C
        C.size++;
        double r_a = squaredDistance(g,p.point); //distancia
        r = ( r > r_a) ? r : r_a;//recalculamos r
    }
    C.size = array_length(C.entries);
    Entry *ret = (Entry *)malloc(sizeof(Entry)); //creamos entry de hoja
    ret->point = g;
    ret->radius = r;
    ret->subTree = &C;
    C.parent = ret;
    return *ret; //se retorna
}



//VER LOS FREE
static Cluster cluster_union(Cluster c1, Cluster c2) {
    //printf("In cluster_union:\n");
    //show_cluster(c1);
    //show_cluster(c2);
    if (array_length(c1.array) == 0) {
        Cluster cluster_res = {c2.array,c2.index_primary_medoide};
        return cluster_res;
    }
    if (array_length(c2.array) == 0) {
        Cluster cluster_res = {c1.array,c1.index_primary_medoide};
        return cluster_res;
    }

    Point * points = get_clusters_points(c1,c2);
    Point medoid = primary_medoide(points);
    int index_medoid = 0;
    for (int i=0; i < array_length(points); i++) {
        if (compare(medoid,points[i])) {
            index_medoid = i;
        }
    }
    Cluster cluster_res = {points,index_medoid};
    //printf("resultado de la unión:\n");
    //show_cluster(cluster_res);
    //array_free(c1.array);
    //array_free(c2.array);
    //printf("Free se salta\n");
    return cluster_res;
}

static int compare_clusters(Cluster c1, Cluster c2) {
    if (array_length(c1.array) != array_length(c2.array)) {
        return FALSE;
    }
    for (int i = 0; i<array_length(c1.array); i++) {
        if (!compare(c1.array[i],c2.array[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

static void cluster_remove(Cluster c1, Cluster *c2) {
    for (int i=0; i < array_length(c2); i++) {
        if (compare_clusters(c2[i],c1)) {
            array_remove(c2,i);
            break;
        }
    }
}

static int find_closest_to(Point * points, Point pivot) {
    float min =INFINITY;
    int closest = 0;
    for(int i=0; i < array_length(points); i++) {
        if(squaredDistance(points[i], pivot) < min) {
            min = squaredDistance(points[i], pivot);
            closest= i;
        }
    }
    return closest;
}

//funcion para buscar el vecino mas cercano a c en cout
static Cluster cluster_nearest_neighbor(Cluster * Cout, Cluster c) {
    Cluster nearest_to_c = {NULL,0};
    float min = INFINITY;
    Point c_medoid= c.array[c.index_primary_medoide]; //guardamos su medoide
    //ahora en los medoides de Cout q son los guardados en C_prima queremos encontrar el cluster mas cercano a c
    for (int i=0;i<array_length(Cout);i++) {
        float new_min = squaredDistance(Cout[i].array[Cout[i].index_primary_medoide],c_medoid);
        if ( new_min < min) {
            nearest_to_c = Cout[i];
            min = new_min;
        }
    }
    return nearest_to_c;
}

static Point * get_clusters_points(Cluster c1, Cluster c2) {
    Point * all_points = array(Point,&my_other_allocator);
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

static double cluster_covering_radius(Cluster c) {
    Point medoide = c.array[c.index_primary_medoide];
    double radius = 0;
    for (int i=0; i<array_length(c.array);i++) {
        double distance = squaredDistance(medoide,c.array[i]);
        radius = (distance>radius)? distance : radius;
    }
    return radius;
}

static Cluster * make_cluster(Point p1, Point p2, Point * points) {
    Point * c1_points = array(Point,&my_other_allocator);
    Point * c2_points = array(Point,&my_other_allocator);
    Point * points_copy = array(Point, &my_other_allocator);
    for (int i=0; i<array_length(points_copy); i++) {
        array_append(points_copy,points[i]);
    }
    int flag = TRUE;
    array_append(c1_points,p1);
    array_append(c2_points,p2);
    while (array_length(points_copy)!=0) {
        if (flag) {
            int index = find_closest_to(points_copy,p1);
            array_append(c1_points,points[index]);
            array_remove(points_copy,index);
            flag = FALSE;
        }
        else {
            int index = find_closest_to(points_copy,p2);
            array_append(c2_points,points[index]);
            array_remove(points_copy,index);
            flag = TRUE;
        }
    }
    Cluster c1 = {c1_points,0};
    Cluster c2 = {c2_points, 0};
    Cluster *clusters = (Cluster *)malloc(sizeof(Cluster)*2);
    clusters[0] = c1;
    clusters[1] = c2;
    return clusters;
}
/* input: c1,c2 clusters
 *  1. all_points como todos los puntos en c1 y c2
 *  2. liberamos memoria de c1 y c2
 *  3. agarrar dos puntos aleatorios
 *  4. crear los clusters de cada punto
 *  5. comparar distancias de los radios y quedarnos con el r_max
 *  6. r_max se compara con el r_min_max y se guarda de ser menor a este
 */
static Cluster *min_max_policy(Cluster c1, Cluster c2) {
    Cluster *result = (Cluster *)malloc(sizeof(Cluster)*2);
    Point * all_points = get_clusters_points(c1,c2); //la unión de c1 y c2
    //array_free(c1.array);
    //array_free(c2.array);
    Point random_points[2];
    double r_min_max = INFINITY;
    //{p1,p2,p3,p4,p5,p6...}
    for (int i = 0; i<array_length(all_points)-1; i++) {
        int j = i+1;
        while (j < array_length(all_points)) {
            random_points[0] = all_points[i];
            random_points[1] = all_points[j];
            Cluster *clusters;
            clusters = make_cluster(random_points[0],random_points[1],all_points);
            double r1 = cluster_covering_radius(clusters[0]);
            double r2 = cluster_covering_radius(clusters[1]);
            double r_max = (r1>r2)? r1:r2;
            if (j == 1) {
                r_min_max = r_max;
                result = clusters;
            }
            if (r_max<r_min_max) {
                //free(result[0].array);
                //free(result[1].array);
                r_min_max = r_max;
                result = clusters;
            }
            j++;
        }
    }
    return result;
}

static Cluster * nearest_clusters(Cluster * clusters) {
    if (array_length(clusters) <= 2) {
        return clusters;
    }
    if (array_length(clusters) <= 3) {
        printf("CASO BORDE LARGO DE C = 3\n");
        for (int i = 0; i<array_length(clusters);i++) {
            show_cluster(clusters[i]);
        }
    }
    Point * medoide_array = array(Point,&my_other_allocator);
    for (int i = 0; i<array_length(clusters);i++) {
        array_append(medoide_array,clusters[i].array[clusters[i].index_primary_medoide]);
    }
    printf("Clusters Points tamano %lu\n",array_length(medoide_array));
    ClosestPoints points = closest(medoide_array, array_length(medoide_array));
    printf("Puntos cercanos: (%f,%f) (%f,%f)\n",points.point1.x,points.point1.y,points.point2.x,points.point2.y);
    Cluster * nearest_clusters = array(Cluster,&my_other_allocator);
    int flag1 = TRUE, flag2= TRUE;
    for (int i = 0; i<array_length(clusters); i++) {
        Point other_point = clusters[i].array[clusters[i].index_primary_medoide];
        if (compare(other_point,points.point1) & flag1) {
            array_append(nearest_clusters, clusters[i]);
            flag1 = FALSE;
        }
        else if(compare(other_point,points.point2) & flag2) {
            array_append(nearest_clusters, clusters[i]);
            flag2 = FALSE;
        }
        else if (!flag1 & !flag2) {
            break;
        }
    }
    return nearest_clusters;
}

/*
 * Metodo para ver el contenido de un cluster
*/
void show_cluster(Cluster c) {
    printf("Elementos del cluster: {");
    for (int i = 0; i<array_length(c.array);i++) {
        printf(" (%f,%f)",c.array[i].x,c.array[i].y);
    }
    printf("} medoide primario: (%f,%f) \n",c.array[c.index_primary_medoide].x,c.array[c.index_primary_medoide].y);
}

//Metodo para Cluster
static Cluster * cluster(Point* input) { // ver si nos dan "n"
    Cluster *C = array(Cluster, &my_other_allocator); //array de puntos c
    Cluster *Cout = array(Cluster, &my_other_allocator); //array de puntos cout
    for(int i=0; i < array_length(input); i++) {
        Point *subset = array(Point, &my_other_allocator); // creamos un sub conjunto de puntos {}
        array_append(subset, input[i]); // se agrega el input[i]
        Cluster sub_cluster = {subset, 0}; // cremos el cluster ({p},0)
        array_append(C, sub_cluster); //se añaden los subconjutos con cada punto del input a C
    }//C = {({p1}, 0), ({p2}, 0), ({p3}, 0) ...}
    while(array_length(C) > 1) { //mientras el tamaño de C sea mayor a 1 es decir mientras podemos separar
        printf("-------------- TAMANO DE C : %lu ----------------------\n",array_length(C));
        /*
        for (int i = 0; i<array_length(C);i++) {
            show_cluster(C[i]);
        }
        */
        printf("Yoinks1\n"); // LO DEJAMOS AQUIIIII
        Cluster *nearest_clusters_values = nearest_clusters(C);
        Cluster cluster1 = nearest_clusters_values[0],cluster2 = nearest_clusters_values[1]; //luego buscamos a que cluster corresponden
        Cluster c1 = (array_length(cluster1.array) > array_length(cluster2.array))? cluster1: cluster2; //guardamos en c1 el cluster de mayor tamaño
        Cluster c2 = (array_length(cluster1.array) <= array_length(cluster2.array))? cluster1: cluster2;
        printf("Yoinksantes\n");
        printf("Tamano de c1: %lu tamano de c2: %lu\n",array_length(c1.array),array_length(c2.array));
        if (array_length(c1.array) + array_length(c2.array) <=B) { //si la suma de tamaño de arreglos es menor a lo q cabe en un bloque
            printf("Yoinksif\n"); // LO DEJAMOS AQUIIIII
            cluster_remove(c1,C); //sacamos los clusters de c
            cluster_remove(c2,C);
            Cluster new_cluster = cluster_union(c1,c2); //los unimos
            array_append(C,new_cluster); //y los añadimos unidos a c
            /*printf("---------C RESULTADO\n");
            for (int i = 0; i<array_length(C);i++) {
                show_cluster(C[i]);
            }*/
        }
        else {
            printf("Yoinkselse\n");
            cluster_remove(c1,C); //si no quitamos el mas grande
            array_append(Cout,c1); //y lo agregamos solito a c
            printf("---------C y Cout RESULTADO\n");
            /*
            printf("--C\n");
            for (int i = 0; i<array_length(C);i++) {
                show_cluster(C[i]);
            }
            */
            printf("--Cout\n");
            for (int i = 0; i<array_length(Cout);i++) {
                show_cluster(Cout[i]);
            }
        }
    }
    printf("Yoinksq\n");
    printf("largo de C: %lu\n",array_length(C));
    Cluster c = C[0]; //se define c como el unico elemento que queda en c
    show_cluster(c);
    Cluster *c_prima = array(Cluster,&my_other_allocator); //iniciamos c prima nulo
    //{ cluster(array,index), clusters(array,index)} c_prima[i]
    printf("TAMANO DE C_PRIMA: %lu \n",array_length(c_prima));
    if (array_length(Cout) >0) { //si el tamaño de cout es mayor a 0
        printf("Yoiks entra 5.\n");
        Cluster c_pivote = cluster_nearest_neighbor(Cout,c); //buscamos en cout el vecino mas cercano a c
        array_append(c_prima,c_pivote);
        cluster_remove(c_pivote,Cout);
    }
    else {
        Cluster c_pivote = {array(Point,&my_other_allocator),0};
        array_append(c_prima,c_pivote);
    }
    // { } { cluster(array,index)}
    printf("--Cout\n");
    for (int i = 0; i<array_length(Cout);i++) {
        show_cluster(Cout[i]);
    }
    printf("--C prima\n");
    for (int i = 0; i<array_length(c_prima);i++) {
        show_cluster(c_prima[i]);
    }
    if ((array_length(c.array) + array_length(c_prima[0].array))<=B) {
        printf("PASO 6\n");
        Cluster new_cluster = cluster_union(c,c_prima[0]);
        printf("PASO 6.1\n");
        array_append(Cout,new_cluster);
    }
    else {
        printf("PASO 6.2\n");
        Cluster *final_clusters = min_max_policy(c,c_prima[0]);
        Cluster c1 = final_clusters[0], c2 = final_clusters[1];
        array_append(Cout,c1);
        array_append(Cout,c2);
    }
    printf("PASO 7\n");
    return Cout; //{ ({p1,p2,3},0),({p4,5},1)...}
}
/**
 *  @brief  Función que retorna todos los puntos de un arreglo de entries
 */
static Point * entries_get_points(Entry * entries) {
    Point * points = array(Point, &my_other_allocator);
    for (int i = 0; i<array_length(entries); i++) {
        array_append(points,entries[i].point);
    }
    return points;
}

/**
 * @brief Función que retorna un entry (G,R,A)
 */
static Entry internal(Tree* c_mra) {
    printf("Internal 1.\n");
    Point * c_in = entries_get_points(c_mra->entries); // Agrupo solo los puntos de las entries
    Point g = primary_medoide(c_in); // Encuentro el medoide primario de este
    double r = 0.0; // Seteo el radio en 0
    printf("Internal 2.\n");
    printf("c_mra tamano: %lu y c_in tamano: %lu",array_length(c_mra->entries),array_length(c_in));
    for (int i = 0; i<array_length(c_in); i++) {
        r = fmax(r,squaredDistance(g,c_in[i]) + c_mra->entries[i].radius); // Voy actualizando la R con cada punto de c_in
    }
    printf("Internal 3.\n");
    Entry result = {g,r,c_mra}; // Creo el entry de salida
    Entry * c = array(Entry,&my_other_allocator); // creo el arreglo final
    array_append(c,result); // añado el resultado al arreglo c, que va a corresponder al padre del nodo
    c_mra->parent = c; // seteo c como el padre del arreglo de entries c_mra
    printf("Internal 4.\n");
    return result;
}

static Entry * entry_filter(Cluster c, Entry *p) {
    Entry * final = array(Entry, &my_other_allocator);
    for (int i = 0; array_length(c.array); i++) {
        for (int j = 0; array_length(p);j++) {
            if (&c.array[i] == &p[j].point) {
                array_append(final,p[j]);
                break;
            }
        }
    }
    return final;
}

Tree * sexton_swinbank(Point * input) {
    /*
     * Caso base
     */
    if (array_length(input)<=B) {
        printf("Caso base\n");
        //Tree * final_tree = array(Tree,&my_other_allocator);
        //array_append(final_tree,*final.subTree);
        //final_tree->height=1;
        //final_tree->size = array_length(input);
        return leaf(input).subTree;
    }
    printf("Salta el caso base\n");
    Cluster * c_out = cluster(input);
    printf("SS 2.\n");
    Entry * c_entries = array(Entry, &my_other_allocator);
    printf("SS 3.\n");
    for (int i = 0; i<array_length(c_out); i++) {
        array_append(c_entries,leaf(c_out[i].array));
    }
    Tree c = {1,array_length(c_entries),c_entries,NULL};
    printf("SS 4.\n");
    while (c.size > B) {
        printf("SS 4.1\n");
        input = entries_get_points(c.entries);
        c_out = cluster(input);
        Tree * c_mra = array(Tree ,&my_other_allocator); //ver sis e pueden cambiar por trees
        for (int i=0; i<array_length(c_out);i++) {
            Entry * s = entry_filter(c_out[i],c.entries);
            Tree s_tree = {c.height,array_length(s), s,NULL};
            array_append(c_mra,s_tree);
        }
        c.size = 0;
        array_free(c.entries);
        c.entries = array(Entry,&my_other_allocator);
        for (int i = 0; i<array_length(c_mra);i++) {
            array_append(c.entries,internal(&c_mra[i]));
        }
        c.size = array_length(c.entries);
        c.height+=1;
    }
    printf("SS 5.\n");
    const Entry final_entry = internal(&c);
    Entry *final_entry_ptr = malloc(sizeof(Entry));
    *final_entry_ptr = final_entry;
    printf("SS 6.\n");
    Point p = (final_entry_ptr->subTree)->entries[0].point;
    double r = (final_entry_ptr->subTree)->entries[0].radius;
    printf("PRIMER ENTRY ANTES DE SALIR: ((%f,%f),%f,NULL)",p.x,p.y,r);
    return final_entry_ptr->subTree;
}