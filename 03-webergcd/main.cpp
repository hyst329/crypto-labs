#include "ishm.h"
#include "weber.h"

int main() {
  gmp_randclass gen(gmp_randinit_default);
  gen.seed(time(0));
  for (int bits : {50, 200, 500, 1000, 2000, 5000, 10000, 40000}) {
    mpz_class q1, q2, g, i;
    q1 = gen.get_z_bits(bits);
    q2 = gen.get_z_bits(bits);
    steady_clock::time_point start = steady_clock::now();
    mpz_class n1 = q1, n2 = q2;
    int p1 = 0, p2 = 0, iter;
    while (n1 % 2 == 0) {
      n1 >>= 1;
      p1++;
    }
    while (n2 % 2 == 0) {
      n2 >>= 1;
      p2++;
    }
    /// cout << q1 << ' ' << q2 << endl;
    g = weber_gcd(n1, n2, 64, iter) * (1_mpz << min(p1, p2));
    // cout << g << endl;
    duration<double> d = steady_clock::now() - start;
    cout << bits << "bits Weber GCD: \t" << d.count() << " (" << iter
    << " iterations)" << endl;
    start = steady_clock::now();
    i = ishm_gcd(n1, n2, 6, iter) * (1_mpz << min(p1, p2));
    d = steady_clock::now() - start;
    cout << bits << "bits Ishmuhametov GCD: \t" << d.count() << " (" << iter
     << " iterations)" << endl;
  }
}
