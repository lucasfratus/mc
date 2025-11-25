#include <stdio.h>
#include <math.h>
#include <stdint.h>


// Provavelmente não utilizaremos essa versão, já que não manipula via IEEE-754
double pow_int(double base, int exp) {
    if (exp == 0) {
        return 1.0; // qualquer coisa elevada a 0 = 1
    }

    int e = exp;
    double b = base;
    double result = 1.0;

    int negativo = 0;
    if (e < 0) {
        negativo = 1;
        e = -e;
    }

    while (e > 0) {
        if (e & 1) {    // se o bit menos significativo de "e" é 1
            result *= b; // multiplica por b
        }
        b *= b; // sempre eleva b ao quadrado
        e >>= 1; // divide e por 2 (shift right)
    }

    if (negativo) {
        result = 1.0 / result; // se o expoente era negativo -> 1/(base^|exp|)
    }

    return result;
}


// Via IEEE-754

// União para acessar os bits de um double
typedef union {
    double d;
    uint64_t u;
} DoubleBits;

// Gera exatamente 2^k manipulando o expoente IEEE-754
double pow2_int(int k) {
    const int bias = 1023;
    int expo_guardado = k + bias; // valor que vai para o campo do expoente

    // tratamento de underflow/overflow
    if (expo_guardado <= 0) {
        return 0.0; // muito pequeno -> aproxima para 0
    }
    if (expo_guardado >= 0x7FF) {
        return INFINITY; // muito grande -> infinito
    }

    DoubleBits db;
    db.u = 0ULL; // zera todos os bits

    // coloca o expoente nos 11 bits de expoente (62 - 52)
    db.u |= ((uint64_t)expo_guardado & 0x7FFULL) << 52;

    // sinal = 0, mantissa = 0
    // => valor = 1.0 * 2^(expo_guardado - bias) = 2^k
    return db.d;
}
