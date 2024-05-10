#include "../structures.h"
#include "../utils.h"
#include "time.h"

Allocator alloc = {my_alloc, my_free, 0};
void query(const Tree tree, const Point point, const double radius, Point ** result, int * accesses){
    double sqr_radius = radius*radius;
    *accesses += 1;
    for (int i = 0; i < tree.size; i++){
        Entry entry = tree.entries[i];
        double distance = squaredDistance(point, entry.point);
        if(distance > sqr_radius + entry.radius)
            continue;

        if(entry.subTree == NULL)
            array_append(*result,entry.point);
        else
            query(*entry.subTree, point, radius, result, accesses);
    }
}

//cambiarle la firma a hundred_queries(Tree tree_cp, Tree tree_ss, int n)
static void hundred_queries(Tree tree,int n){
    for(int i = 0; i < 100; i++){
        Point * results = array(Point, &alloc); //arreglo para arbol cp
        int accesses = 0; //accesos en árbol cp
        Point center = generateRandomPoint(); //este punto se usa para ambos árboles!!!

        query(tree, center, 0.02, &results, &accesses);
        printf("Query %i finished:\n",i + 1);
        printf("accesses_cp = %i\n", accesses);
        printf("points = %zu\n", array_length(results));
        printf("point percentage = %f \n", (double)array_length(results) / (double)n * 100);
        printf("\n");

        array_free(results);
    }
}

void genarate_experiment_cp(void) {
    int input_sizes[] = {1024,2048,4096, 8192, 16384, 32768, 65536, 131072, 262144,
                         524288, 1048576, 2097152, 4194304, 8388608, 16777216, 33554432};
    for(int i = 0; i < 16; i++){
        int n = input_sizes[i];

        Point *input = array(Point, &alloc);

        for(int j = 0; j < n; j++)
            array_append(input, generateRandomPoint());

        printf("%i random points of double precision generated. \n",n);
        struct timespec t1, t2;

        clock_gettime(CLOCK_MONOTONIC,&t1);

        Tree * tree_cp = ciaccia_patella(input);

        printf("Tree of height %i built using Ciaccia-Patella.\n", tree_cp->height);

        printf("starting queries...\n");

        hundred_queries(*tree_cp, n);

        clock_gettime(CLOCK_MONOTONIC,&t2);

        double delta_t;
        delta_t = (t2.tv_sec - t1.tv_sec) * 1e9;
        delta_t = (delta_t + (t2.tv_nsec - t1.tv_nsec)) * 1e-9;

        printf("Execution time: %f seconds \n", delta_t);
        printf("\n");

        free_tree(tree_cp);

        array_free(input);
    }
}
void genarate_experiment_ss() {
    int input_sizes[] = {1024,2048,4096, 8192, 16384, 32768};
    for(int i = 0; i < 6; i++){
        int n = input_sizes[i];

        Point *input = array(Point, &alloc);

        for(int j = 0; j < n; j++)
            array_append(input, generateRandomPoint());

        printf("%i random points of double precision generated. \n",n);
        struct timespec t1, t2;

        clock_gettime(CLOCK_MONOTONIC,&t1);


        Tree * tree_ss = sexton_swinbank(input);

        printf("Tree of height %i built using Sexton-Swinbank.\n", tree_ss->height);

        printf("starting queries...\n");

        hundred_queries(*tree_ss, n);

        clock_gettime(CLOCK_MONOTONIC,&t2);

        double delta_t;
        delta_t = (t2.tv_sec - t1.tv_sec) * 1e9;
        delta_t = (delta_t + (t2.tv_nsec - t1.tv_nsec)) * 1e-9;

        printf("Execution time: %f seconds \n", delta_t);
        printf("\n");

        free_tree(tree_ss);

        array_free(input);
    }
}

void generate_experiment(){
    genarate_experiment_cp();
    genarate_experiment_ss();
}