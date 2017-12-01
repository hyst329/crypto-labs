#include <chrono>
#include <gmp.h>
#include <gmpxx.h>
#include <iostream>
#include <map>
#include <random>

using namespace std;
using namespace std::chrono;

int bit_length(mpz_class x) {
  if (x < 0)
    x = -x;
  int r = 0;
  while (x > 0) {
    x >>= 1;
    r += 1;
  }
  return r;
}

mpz_class divide(mpz_class u, mpz_class v, int s) {
  mpz_class d = u, m = 0;
  for (int i = 0; i < s; i++) {
    if (d % (1_mpz << (i + 1)) != 0) {
      d -= (1_mpz << i) * v;
      m += (1_mpz << i);
    }
  }
  return m;
}

pair<mpz_class, mpz_class> reduced_rat_mod(mpz_class x, mpz_class y, int h) {
  mpz_class c = divide(x, y, 2 * h);
  pair<mpz_class, mpz_class> f1 = {1_mpz << (2 * h), 0};
  pair<mpz_class, mpz_class> f2 = {c, 1};
  while (f2.first >= 1_mpz << h) {
    f1 = {f1.first - (f1.first / f2.first) * f2.first,
          f1.second - (f1.first / f2.first) * f2.second};
    swap(f1, f2);
  }
  return f2;
}

mpz_class dmod(mpz_class u0, mpz_class v0, int w) {
  mpz_class u = u0;
  while (bit_length(u) >= bit_length(v0) + w) {
    if (u % (1_mpz << w) != 0) {
      u = abs(u - divide(u, v0, w) * v0);
      u >>= w;
    }
  }
  int d = bit_length(u) - bit_length(v0);
  if (u % (1_mpz << max(d + 1, 0)) != 0) {
    u = abs(u - divide(u, v0, d + 1) * v0);
  }
  return u >> max(d + 1, 0);
}

mpz_class weber_gcd(mpz_class u0, mpz_class v0, int w) {
  if (u0 < v0)
    swap(u0, v0);
  auto s = [](mpz_class v) { return 64; };
  auto t = [](mpz_class v) { return 192; };
  mpz_class u = u0, v = v0;
  while (v != 0) {
    // cout << u << " " << v << endl;
    if (bit_length(u) - bit_length(v) > s(v)) {
      u = dmod(u, v, w);
    } else {
      auto p = reduced_rat_mod(u, v, t(v));
      mpz_class n = p.first, d = p.second;
      u = abs(n * v - d * u) >> (2 * t(v));
    }
    while ((u % 2 == 0) && u) {
      u /= 2;
    }
    swap(u, v);
  }
  mpz_class x = gcd(dmod(v0, u, w), u);
  return gcd(dmod(u0, x, w), x);
}

int main() {
  gmp_randclass gen(gmp_randinit_default);
  gen.seed(time(0));
  for (int bits : {50, 200, 500, 1000, 2000, 5000, 10000, 40000}) {
    mpz_class q1, q2, g, h;
    q1 = gen.get_z_bits(bits);
    q2 = gen.get_z_bits(bits);
    steady_clock::time_point start = steady_clock::now();
    mpz_class n1 = q1, n2 = q2;
    int p1 = 0, p2 = 0;
    while (n1 % 2 == 0) {
      n1 >>= 1;
      p1++;
    }
    while (n2 % 2 == 0) {
      n2 >>= 1;
      p2++;
    }
    /// cout << q1 << ' ' << q2 << endl;
    g = weber_gcd(n1, n2, 64) * (1_mpz << min(p1, p2));
    // cout << g << endl;
    duration<double> d = steady_clock::now() - start;
    cout << bits << "bits Weber GCD: \t" << d.count() << endl;
    start = steady_clock::now();
    h = gcd(q1, q2);
    d = steady_clock::now() - start;
    cout << bits << "bits GMP's GCD: \t" << d.count() << endl;
    if (g != h)
      cout << "Error detected" << endl;
  }
}
