#include <chrono>
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <map>
#include <random>

#ifndef WEBER_H
#define WEBER_H

using namespace std;
using namespace std::chrono;

int bit_length(mpz_class x);
mpz_class divide(mpz_class u, mpz_class v, int s);
pair<mpz_class, mpz_class> reduced_rat_mod(mpz_class x, mpz_class y, int h);
mpz_class dmod(mpz_class u0, mpz_class v0, int w);
mpz_class weber_gcd(mpz_class u0, mpz_class v0, int w, int& iter);

#endif // WEBER_H
