#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define TOTAL_BOLAS 10
// Implementación de un LFSR de 8 bits
uint8_t lfsr(uint8_t seed) {
    // El polinomio utilizado es x^8 + x^6 + x^5 + x^4 + 1
    uint8_t feedback = (seed >> 7) ^ (seed >> 5) ^ (seed >> 4) ^ 1;
    seed = (seed << 1) | feedback;
    return seed%100;
}

int main() {
    uint8_t seed = 0x41;  // Puedes elegir cualquier estado inicial no cero

    for (int i = 0; i < 10; ++i) {
        seed = lfsr(seed);
        printf("Número pseudoaleatorio: %u\n", seed);
    }

    return 0;
}

