#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>


#define k2  -0.50000000000000000000
#define k3  -0.16666666666666666666
#define k4   0.04166666666666666666
#define k5   0.00833333333333333333

#define k6  -1.388888888888889E-03
#define k7  -1.984126984126984E-04
#define k8   2.480158730158730E-05
#define k9   2.755731922398589E-06

#define k10  -2.755731922398589E-07
#define k11  -2.505210838544172E-08
#define k12   2.087675698786810E-09

#define PI 3.14159265358979323846264
#define PI4 (PI/4.0)
#define PI2 (PI/2.0)
#define dPI (PI*2.0)

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

// sen(x), |x| <= PI/4
static double serie_seno(double x){
    double y = x * x;
    return x * (1 + y*(k3 + y*(k5 + y*(k7 + y*(k9 + y*k11)))));
} 

// cos(x), |x| <= PI/4
static double serie_cos(double x){
    double y = x * x;
    return 1 + y* (k2 + y*(k4 + y*(k6 + y*(k8 + y*(k10 + y*k12)))));
}
