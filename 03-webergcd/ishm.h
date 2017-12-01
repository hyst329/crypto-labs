#include <chrono>
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <map>
#include <random>

#ifndef ISHM_H
#define ISHM_H

mpz_class ishm_gcd(mpz_class a, mpz_class b, int l, int& iter); // k = 2 ^ l

#endif // ISHM_H
