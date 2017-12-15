#include <chrono>
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <map>
#include <random>

#ifndef ISHM_H
#define ISHM_H

using namespace std;

int bit_length(mpz_class x);
mpz_class ishm_gcd(mpz_class a, mpz_class b, int l, int &iter); // k = 2 ^ l
tuple<mpz_class, mpz_class, mpz_class> single_iter(mpz_class a, mpz_class b,
                                                   int l);

#endif // ISHM_H
