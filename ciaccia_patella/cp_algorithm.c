#include <math.h>
#include <malloc.h>
#include "../structures.h"
#include "../utils.h"
#include "../arrays/array.h"
#include "../arrays/matrix.h"
#include "../trees/2dTrees.h"
#include "../random/custom_random.h"

Allocator my_allocator = {my_alloc, my_free, 0};

/*            Paso 1            */
static Tree * leaf(Point * input){
    Tree * T = malloc(sizeof(Tree));
    T->height = 1;
    T->size = array_length(input);
    T->entries = array(Entry,&my_allocator);
    T->parent = NULL;

    for(int i = 0; i < array_length(input); i++){
        Entry entry = {input[i],0,NULL};
        array_append(T->entries,entry);
    }

    return T;
}

/*                      Paso 2                          */
static Point* build_sample_set(Point * input, int expected_size){
    Point *output = array(Point, &my_allocator);
    BTree2D *binary_Tree = initBT(1);

    while(array_length(output) != expected_size){
        unsigned int random_index = splitmix32_next() % array_length(input);
        Point sample = input[random_index];

        int is_in_BT = searchBT2D(binary_Tree, sample);

        if(is_in_BT)
            continue;

        (&sample)->index = array_length(output);
        insertBT2D(binary_Tree, input[random_index]);
        array_append(output,sample);
    }
    freeBT2D(binary_Tree);
    return output;
}

/*                               Paso 3                                  */
static void match_with_closest_sample(Point **sets, Point *input, Point* samples){
    BTree2D *binary_tree = initBT(1);

    for(int i = 0; i < array_length(samples); i++)
        insertBT2D(binary_tree, samples[i]);

    for (int i = 0; i < array_length(input); i++){
        Point current_point = input[i];
        Point closest;
        findClosestBT2D(binary_tree, current_point, INFINITY, &closest);
        matrix_append(sets, closest.index, current_point);
    }

    freeBT2D(binary_tree);
}

/*                    Paso 4                     */
static void redistribution(Point ** sets, Point* samples){
    int i = 0;

    while(i < array_length(samples)){
        if (array_length(sets[i]) >= b){
            i++;
            continue;
        }

        Point * points_in_row = sets[i];
        array_remove(samples, i);
        matrix_remove_row(sets, i);

        for(int j = 0; j < array_length(samples); j++)
            (&samples[j])->index = j;

        match_with_closest_sample(sets, points_in_row, samples);
        array_free(points_in_row);
    }
}

/*                          Paso 6                               */
static Tree * recursive_step(Point * samples, Point **samples_sets){
    Tree * trees = array(Tree, &my_allocator);

    for(int i = 0; i < array_length(samples); i++) {
        if (array_length(samples_sets[i]) == 0)
            continue;

        Tree *root = ciaccia_patella(samples_sets[i]);
        array_append(trees, *root);
    }

    return trees;
}

/*               Paso 8               */
static int get_min_height(Tree * trees){
    int height = INFINITY;

    for(int i = 0; i < array_length(trees); i++){
        if (trees[i].size >= b){
            height = (trees[i].height < height) ? trees[i].height : height;
            continue;
        }

         for (int j = 0; j < trees[i].size; j++){
             Entry entry = trees[i].entries[j];
             height = (entry.subTree->height < height) ? entry.subTree->height : height;
         }
    }
    return height;
}


static void find_subtrees_by_height(Tree* tree, int height, Tree **tree_output, Point ** point_output){
    if (tree == NULL)
        return;

    for(int i = 0; i < tree->size; i++){
        Entry current = tree->entries[i];

        if(current.subTree == NULL)
            continue;
        if (current.subTree->height < height)
            continue;
        if(current.subTree->height > height){
            find_subtrees_by_height(current.subTree, height, tree_output, point_output);
            continue;
        }

        array_append(*tree_output, *current.subTree);
        array_append(*point_output, current.point);
    }
}

static void check_height(Tree * tree, Point sample, int height, Tree **T, Point **F){
    if(tree->height == height){
        array_append(*T,*tree);
        array_append(*F,sample);
        return;
    }
    find_subtrees_by_height(tree, height, T, F);
}

/*                               Pasos 7 y 9                                     */
static void balancing(Tree *trees, Point *samples, int height, Tree **T, Point **F){
    for(int i = 0; i < array_length(trees); i++){
        int root_cut = FALSE;
        if (trees[i].size < b)
            root_cut = TRUE;

        if(root_cut == FALSE){
            check_height(&(trees[i]), samples[i], height, T, F);
            continue;
        }

        for(int j = 0; j < trees[i].size; j++){
            Entry entry = trees[i].entries[j];
            if(entry.subTree == NULL)
                continue;
            check_height(entry.subTree, entry.point, height, T, F);
        }
    }
}

static double entry_radius(Tree * subtree, Point point){
    double max = 0;

    for(int i = 0; i < subtree->size; i++){
        double distance = squaredDistance(point, subtree->entries[i].point);
        max = (distance > max) ? distance : max;
    }

    return sqrt(max);
}

static void set_height(Tree *T_sup){
    if(T_sup == NULL)
        return;

    int max_height = 0;

    for(int i = 0; i < T_sup->size; i++){
        Tree * subtree = T_sup->entries[i].subTree;
        set_height(T_sup->entries[i].subTree);
        if(subtree == NULL)
            continue;

        max_height = (subtree->height > max_height) ? subtree->height : max_height;
    }
    T_sup->height = max_height + 1;
}

/*                       Paso 11                           */
static void tree_merge(Tree *T_sup, Tree *tree, Point sample){
    if(T_sup == NULL)
        return;

    for(int i = 0; i < T_sup->size; i++){
        Entry *entry = &(T_sup->entries[i]);
        if(sample.x == entry->point.x
        && sample.y == entry->point.y
        && entry->subTree == NULL){
            tree->parent = entry;
            return;
        }
    }
    for(int i = 0; i < T_sup->size; i++)
        tree_merge(T_sup->entries[i].subTree, tree, sample);
}


void free_tree(Tree *tree){
    if (tree == NULL)
        return;

    for(int i = 0; i < tree->size; i++){
        Entry *entry = &(tree->entries[i]);
        Tree *p = entry->subTree;
        entry->subTree = NULL;
        free_tree(p);
    }
    array_free(tree->entries);
    free(tree);
}

/*           Algoritmo             */
Tree * ciaccia_patella(Point * input){
    int n = array_length(input);

    if(n <= B)
        return leaf(input);

    int n_div_B = 1 + ((n - 1) / B);
    int k = (B >= n_div_B) ? n_div_B : B;

    Point *F;
    Point **F_sets;

    do{
        F_sets = matrix(Point, k, &my_allocator);
        F = build_sample_set(input, k);
        match_with_closest_sample(F_sets, input, F);
        redistribution(F_sets, F);

        if(array_length(F) > 1)
            break;

        array_free(F);
        matrix_free(F_sets);
    }while(TRUE);

    Tree *temp = recursive_step(F, F_sets);

    Tree * T = array(Tree,&my_allocator);
    Point * F_new = array(Point, &my_allocator);

    int h = get_min_height(temp);
    balancing(temp,F,h, &T, &F_new);

    Tree *T_sup;

    while(TRUE){
        T_sup = ciaccia_patella(F_new);

        for(int i = 0; i < array_length(T); i++)
            tree_merge(T_sup, &T[i], F_new[i]);

        int success = TRUE;

        for(int i = 0; i < array_length(T); i++){
            Entry *p = T[i].parent;
            if(p == NULL){
                success = FALSE;
                break;
            }
        }

        if(success)
            break;

        free_tree(T_sup);
    }

    for(int i = 0; i < array_length(T); i++){
        Entry *p = T[i].parent;
        p->subTree = malloc(sizeof(Tree));
        *(p->subTree) = T[i];
        p->radius = entry_radius(p->subTree, p->point);
    }

    set_height(T_sup);

    array_free(F);
    array_free(F_new);
    matrix_free(F_sets);
    array_free(temp);
    array_free(T);

    return T_sup;
}
