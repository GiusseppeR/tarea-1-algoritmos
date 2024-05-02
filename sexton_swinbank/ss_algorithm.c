#include <math.h>
#include <stdlib.h>
#include "../structures.h"
#include "../utils.h"
#include "../arrays/array.h"
#include "../arrays/matrix.h"
#include "../trees/2dTrees.h"
#include "../random/custom_random.h"

Allocator my_allocator = {my_alloc, my_free, 0};

typedef struct {
    Point g; //medoide
    double *r; //radio cobertor
    void *a; //cambiar, direccion del hijo
}Tuple;
//FALTA VER CENTROIDE
static Point centroid(Point * input) {

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
    //y lurgo veremos el punto más cercano a este
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

//Metodo para Cluster

static Point * cluster(Point* input) {

}



