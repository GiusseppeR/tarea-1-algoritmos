#ifndef TAREALOG_QUERY_H
#define TAREALOG_QUERY_H
#include "../structures.h"
void query(Tree tree, Point point, double radius, Point ** result, int * accesses);
void generate_experiment_cp(Point * input, Point* query_points);
void generate_experiment_ss(Point * input, Point* query_points);

#endif //TAREALOG_QUERY_H
