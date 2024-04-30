#include <stdint.h>

static uint64_t splitmix64_state;
static uint64_t xs_state[4];

void splitmix64_init(uint64_t seed) {
    splitmix64_state = seed;
}

uint64_t splitmix64_next() {
    uint64_t z = (splitmix64_state += UINT64_C(0x9E3779B97F4A7C15));
    z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
    return z ^ (z >> 31);
}
unsigned int splitmix32_next(){
    return (unsigned int) (splitmix64_next() & 0xFFFFFFFF);
}

void xoshiro256plus_init() {
    xs_state[0] = splitmix64_next();
    xs_state[1] = splitmix64_next();
    xs_state[2] = splitmix64_next();
    xs_state[3] = splitmix64_next();
}


static inline uint64_t rotl(const uint64_t x, int k) {
    return (x << k) | (x >> (64 - k));
}

double xoshiro256plus_next(void) {
    const double factor = 1.0 / (UINT64_MAX >> 11);

    const uint64_t result = xs_state[0] + xs_state[3];

    const uint64_t t = xs_state[1] << 17;

    xs_state[2] ^= xs_state[0];
    xs_state[3] ^= xs_state[1];
    xs_state[1] ^= xs_state[2];
    xs_state[0] ^= xs_state[3];

    xs_state[2] ^= t;

    xs_state[3] = rotl(xs_state[3], 45);

    return (double) (result >> 11) * factor;
}