#include "ishm.h"
#include "weber.h"
#include <cassert>

mpz_class cut(mpz_class s, int l) { return s & ~(1_mpz << l - 1); }

mpz_class ishm_gcd(mpz_class a, mpz_class b, int l, int& iter) {
  if (a < b)
    swap(a, b);
  mpz_class k = 1_mpz << l;
  iter = 0;
  while (1) {
    //cout << a << "\t" << b << endl;
    int l1 = bit_length(a);
    int l2 = bit_length(b);
    int d = l1 - l2;
    mpz_class q, c;
    if (d > 2 * l) {
      q = divide(a, b, d);
      c = (a - q * b) >> d;
    } else {
      mpz_class _a = a % l, _b = b % l;
      q = divide(_a, _b, l);
      //cout << "q = " << q << endl;
      int t = l2 - 2 * l;
      if (t <= 0) {
        return gcd(a % b, b);
      }
      mpz_class ap = a >> t, bp = b >> t;
      mpz_class n0 = bp << l;
      mpz_class m1 = ap - q * bp;
      if (m1 < 0) {
        q -= k;
        m1 = ap - q * bp;
      }
      //cout << "n0 = " << n0 << " m1 = " << m1 << endl;
      mpz_class m0 = m1 % n0;
      mpz_class s1 = (m1 - m0) / n0;
      //cout << "m0 = " << m0 << " s1 = " << s1 << endl;
      mpq_class r0 = mpq_class(m0) / n0;
      mpz_class x, y;
      if (r0 < 1_mpq / k) {
        x = 1;
        y = -(q + s1 * k);
      } else {
        mpq_class r1, r2;
        if (r0 < 1_mpq / 2) {
          mpz_class z = n0 / m0;
          r1 = 1_mpq / (z + 1);
          r2 = 1_mpq / z;
        } else {
          mpz_class z = n0 / (n0 - m0);
          r1 = mpq_class(z - 1) / z;
          r2 = mpq_class(z) / (z + 1);
        }
        while (r1.get_den() + r2.get_den() < k) {
          mpq_class r3(r1.get_num() + r2.get_num(), r1.get_den() + r2.get_den());
          ((r3 < r0) ? r1 : r2) = r3;
        }
        //cout << r0 << "\t" << r1 << "\t" << r2 << endl;
        mpz_class m1 = r1.get_num(), m2 = r2.get_num();
        mpz_class n1 = r1.get_den(), n2 = r2.get_den();
        bool q1 = (2 * r0) < (r1 + r2), q2 = n1 < (2 * (k - 1) / 3), q3 = n2 < (2 * (k - 1) / 3);
        bool q4 = ((q1 && q2) || (!q1 && q3));
        mpz_class m = q4 ? m1 : m2, n = q4 ? n1 : n2;
        x = n;
        mpz_class s = -(s1 * x + m);
        y = -q * x + s * k;
      }
      mpz_class a1 = a >> l;
      mpz_class b1 = b >> l;
      mpz_class c1 = (_a * x + _b * y) / k;
      //cout << "x = " << x << " y = " << y << endl;
      c = abs(a1 * x + b1 * y + c1);
    }
    while ((c & 1) == 0 && c != 0) c >>= 1;
    a = b;
    b = c;
    iter++;
  }
}
