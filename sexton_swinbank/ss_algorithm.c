#include <math.h>
#include <stdlib.h>
#include "../structures.h"
#include "../utils.h"
#include "ss_utils.h"
#include "../random/custom_random.h"
#include "../trees/2dTrees.h"

Allocator my_other_allocator = {my_alloc, my_free, 0};

static Point * get_clusters_points(Cluster c1, Cluster c2);

/**
 * @brief Función que retorna el medoide primario dentro de un arreglo de puntos
 */
static Point primary_medoide(Point * input) {
    double * radius = array(double, &my_other_allocator);

    BTree2D *binary_tree = initBT(1);

    for(int i = 0; i < array_length(input); i++)
        insertBT2D(binary_tree, input[i]);

    for (int i = 0; i < array_length(input) - 1; i++){
        Point farthest;
        findFarthestBT2D(binary_tree,input[i],0,&farthest);

        double rad = squaredDistance(input[i],farthest);
        array_append(radius, rad);
    }

    int index = 0;

    double min_rad = INFINITY;
    for (int i = 0; i < array_length(radius); i++){
        if (radius[i] < min_rad){
            min_rad = radius[i];
            index = i;
        }
    }
    freeBT2D(binary_tree);
    return (Point) {input[index].x, input[index].y, index};
}
/**
 * @brief Función que retorna un arreglo dinamico que contiene los puntos copiados de points.
 */
static Point * copy_array(Point * points) {
    Point * final = array(Point,&my_other_allocator);
    for (int i = 0; i<array_length(points); i++) {
        Point p = {points[i].x,points[i].y,0};
        array_append(final,p);
    }
    return final;
}

static Cluster cluster_union(Cluster c1, Cluster c2) {
    if (array_length(c2.array) == 0) {
        Point * points = get_clusters_points(c1,c2);
        Cluster cluster_res = {points,c1.index_primary_medoide};
        return cluster_res;
    }

    Point * points = get_clusters_points(c1,c2);
    Point medoid = primary_medoide(points);
    int index_medoid = medoid.index;

    Cluster cluster_res = {points,index_medoid};
    return cluster_res;
}

static int find_closest_to(Point * points, Point pivot) {
    double min =INFINITY;
    int closest = 0;
    for(int i=0; i < array_length(points); i++) {
        if(squaredDistance(points[i], pivot) < min) {
            min = squaredDistance(points[i], pivot);
            closest= i;
        }
    }
    return closest;
}

static int cluster_nearest_neighbor(Cluster * Cout, Cluster c) {
    int nearest_to_c = 0;
    double min = INFINITY;
    Point c_medoid= c.array[c.index_primary_medoide];

    for (int i=0;i<array_length(Cout);i++) {

        double new_min = squaredDistance(Cout[i].array[Cout[i].index_primary_medoide],c_medoid);
        if ( new_min < min) {
            nearest_to_c = i;
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

    for (int i = 0; i < n; i++) {
        if (i < c1_lenght) {
            Point p = {c1.array[i].x,c1.array[i].y,0};
            array_append(all_points,p);
        }
        if (i < c2_lenght) {
            if (equals(c1.array[i],c2.array[i]))
                continue;
            Point p = {c2.array[i].x,c2.array[i].y,0};
            array_append(all_points,p);
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
    array_append(c1_points,p1);
    array_append(c2_points,p2);

    for (int i = 0; i<array_length(points); i++) {
        if (equals(p1, points[i]) || equals(p2, points[i])) {
            continue;
        }
        array_append(points_copy,points[i]);
    }

    int flag = TRUE;

    while (array_length(points_copy)!=0) {
        if (flag) {
            int index = find_closest_to(points_copy,p1);
            array_append(c1_points,points_copy[index]);
            array_remove(points_copy,index);
            flag = FALSE;
        }
        else {
            int index = find_closest_to(points_copy,p2);
            array_append(c2_points,points_copy[index]);
            array_remove(points_copy,index);
            flag = TRUE;
        }
    }
    Cluster c1 = {c1_points,0};
    Cluster c2 = {c2_points, 0};
    Cluster *clusters = (Cluster *)malloc(sizeof(Cluster)*2);
    clusters[0] = c1;
    clusters[1] = c2;
    array_free(points_copy);
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
    Point random_points[2];

    double r_min_max = INFINITY;
    //{p1,p2,p3,p4,p5,p6...}
    for (int i = 0; i < array_length(all_points) - 1; i++) {
        int j = i+1;
        while (j < array_length(all_points)) {
            random_points[0] = all_points[i];
            random_points[1] = all_points[j];

            Cluster *clusters = make_cluster(random_points[0],random_points[1],all_points);
            double r1 = cluster_covering_radius(clusters[0]);
            double r2 = cluster_covering_radius(clusters[1]);
            double r_max = (r1 > r2) ? r1 : r2;

            if (j == 1) {
                r_min_max = r_max;
                result[0]= clusters[0];
                result[1] = clusters[1];
            }
            else if (r_max < r_min_max) {
                array_free(result[0].array);
                array_free(result[1].array);
                r_min_max = r_max;
                result[0]= clusters[0];
                result[1] = clusters[1];
            }
            else {
                array_free(clusters[0].array);
                array_free(clusters[1].array);
            }

            j++;
        }
    }
    return result;
}
/**
 * @brief Retorna el indice de los clusters más cercanos
 */
static int * nearest_clusters(Cluster * clusters) {
    static int indices[2];
    if (array_length(clusters)==2) {
        indices[0] = 0;
        indices[1] = 1;
        return indices;
    }

    Point * medoide_array = array(Point,&my_other_allocator);
    BTree2D *binary_Tree = initBT(1);

    for (int i = 0; i<array_length(clusters);i++) {
        Point m = clusters[i].array[clusters[i].index_primary_medoide];
        (&m)->index = i;
        array_append(medoide_array,m);
        insertBT2D(binary_Tree, m);
    }

    double min_dist = INFINITY;
    for(int i = 0; i < array_length(medoide_array); i++){
        Point closest;
        findClosestBT2D(binary_Tree,medoide_array[i],INFINITY,&closest,FALSE);

        double dist = squaredDistance(medoide_array[i],closest);
        if(dist < min_dist){
            min_dist = dist;
            indices[0] = i;
            indices[1] = closest.index;
        }
    }

    array_free(medoide_array);
    freeBT2D(binary_Tree);
    return indices;
}


//Metodo para Cluster
Cluster * cluster(Point* input) {
    Cluster *C = array(Cluster, &my_other_allocator);
    Cluster *Cout = array(Cluster, &my_other_allocator);

    for(int i=0; i < array_length(input); i++) {
        Point *subset = array(Point, &my_other_allocator);
        array_append(subset, input[i]);

        Cluster sub_cluster = {subset, 0};
        array_append(C, sub_cluster);
    }//C = {({p1}, 0), ({p2}, 0), ({p3}, 0) ...}

    while(array_length(C) > 1) {
        int *clusters_index = nearest_clusters(C);

        Cluster cluster1 = C[clusters_index[0]];
        Cluster cluster2 = C[clusters_index[1]];

        Cluster c1 = (array_length(cluster1.array) > array_length(cluster2.array))? cluster1: cluster2;
        Cluster c2 = (array_length(cluster1.array) <= array_length(cluster2.array))? cluster1: cluster2;

        int c1_index = (array_length(cluster1.array) > array_length(cluster2.array))? clusters_index[0]:clusters_index[1];
        Cluster new_cluster = cluster_union(c1,c2);
        if (array_length(new_cluster.array) <= B) {
            if (clusters_index[0] > clusters_index[1]) {
                array_remove(C,clusters_index[0]);
                array_remove(C,clusters_index[1]);
            }
            else {
                array_remove(C,clusters_index[1]);
                array_remove(C,clusters_index[0]);
            }
            array_free(c1.array);
            array_free(c2.array);
            array_append(C,new_cluster);
        }
        else {
            Cluster c1_prima = {copy_array(c1.array), c1.index_primary_medoide};
            array_append(Cout,c1_prima);
            array_remove(C,c1_index);

        }
    }

    Cluster c = C[0]; //se define c como el unico elemento que queda en c
    Cluster *c_prima = array(Cluster,&my_other_allocator); //iniciamos c prima nulo

    if (array_length(Cout) > 0) {
        int nearest_i = cluster_nearest_neighbor(Cout,c);
        array_append(c_prima,Cout[nearest_i]);
        array_remove(Cout,nearest_i);
    }
    else {
        Cluster c_pivote = {(Point *)array(Point,&my_other_allocator),0};
        array_append(c_prima,c_pivote);
    }
    Cluster new_cluster = cluster_union(c,c_prima[0]);
    if (array_length(new_cluster.array)<=B) {
        array_append(Cout,new_cluster);
    }
    else {
        Cluster *final_clusters = min_max_policy(c,c_prima[0]);
        Cluster c1 = final_clusters[0], c2 = final_clusters[1];
        array_append(Cout,c1);
        array_append(Cout,c2);
    }
    array_remove(C,0);
    array_free(C);
    array_free(c_prima[0].array);
    array_free(c_prima);
    return Cout; //{ ({p1,p2,3},0),({p4,5},1)...}
}
/**
 *  @brief  Función que retorna todos los puntos de un arreglo de entries
 */
static Point * entries_get_points(Entry * entries) {
    Point * points = array(Point, &my_other_allocator);
    for (int i = 0; i<array_length(entries); i++) {
        Point p = entries[i].point;
        array_append(points,p);
    }
    return points;
}

static Entry leaf(Point * input){
    Point g = primary_medoide(input);
    double r = 0;

    Tree * C = (Tree *)malloc(sizeof(Tree));
    C->height = 1;
    C->size = 0;
    C->entries = array(Entry,&my_other_allocator);
    C->parent = NULL;

    for(int i = 0; i < array_length(input); i++){
        Entry p = {input[i], 0, NULL};
        array_append(C->entries, p);
        C->size += 1;

        double r_a = squaredDistance(g,p.point);
        r = ( r > r_a) ? r : r_a;
    }

    Entry *ret = (Entry *)malloc(sizeof(Entry));
    ret->point = g;
    ret->radius = sqrt(r);
    ret->subTree = C;
    C->parent = ret;
    return *ret;
}

/**
 * @brief Función que retorna un entry (G,R,A)
 */
static Entry internal(Tree* c_mra) {
    Point * c_in = entries_get_points(c_mra->entries);
    Point g = primary_medoide(c_in);
    double r = 0;

    for (int i = 0; i<array_length(c_in); i++) {
        double dist = sqrt(squaredDistance(g,c_in[i]));
        r = fmax(r,dist + c_mra->entries[i].radius);
    }

    Entry result = {g,r,c_mra};
    c_mra->parent = NULL;
    return result;
}

Tree * sexton_swinbank(Point * input) {
    if (array_length(input)<=B) {
        return leaf(input).subTree;
    }
    Cluster * c_out = cluster(input);
    Entry * c_entries = array(Entry, &my_other_allocator);
    for (int i = 0; i<array_length(c_out); i++) {
        array_append(c_entries,leaf(c_out[i].array));
    }
    Tree * c = malloc(sizeof(Tree));
    c->height = 0;
    c->size = array_length(c_entries);
    c->entries = c_entries;
    c->parent = NULL;
    while (c->size > B) {
        input = entries_get_points(c->entries);
        c_out = cluster(input);
        Tree ** c_mra = array(Tree *,&my_other_allocator);
        for (int i=0; i<array_length(c_out);i++) {
            Tree *s_tree = malloc(sizeof(Tree));
            s_tree->height = 0;
            s_tree->size = 0;
            s_tree->entries = array(Entry,&my_other_allocator);
            s_tree->parent = NULL;

            for (int j = 0; j<array_length(c_out[i].array);j++) {
                for(int k = 0; k<array_length(c->entries);k++) {
                    if (equals(c->entries[k].point,c_out[i].array[j])) {
                        Entry entry = {c->entries[k].point,c->entries[k].radius,c->entries[k].subTree};
                        array_append(s_tree->entries,entry);
                        break;
                    }
                }
            }
            s_tree->size = array_length(s_tree->entries);
            array_append(c_mra,s_tree);
        }
        c->size = 0;
        Entry * entrie_pivote = c->entries;
        c->entries = array(Entry,&my_other_allocator);
        for (int i = 0; i<array_length(c_mra);i++) {
            array_append(c->entries,internal(c_mra[i]));
        }
        c->size = array_length(c->entries);
        array_free(entrie_pivote);
    }
    set_height(c);
    return internal(c).subTree;
}