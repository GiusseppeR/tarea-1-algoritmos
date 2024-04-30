
#ifndef TAREALOG_CUSTOM_RANDOM_H
#define TAREALOG_CUSTOM_RANDOM_H

#include <stdint.h>

void splitmix64_init(uint64_t seed);
uint64_t splitmix64_next();
void xoshiro256plus_init();
double xoshiro256plus_next(void);
unsigned int splitmix32_next();

#endif
