#include "ishm.h"
#include <chrono>

using namespace std;

int main() {
  const int q_max = 1024;
  vector<vector<tuple<mpz_class, mpz_class, mpz_class>>> tb1, tb2;
  tb1.resize(q_max);
  tb2.resize(q_max);
  for (int i = 0; i < q_max; i++) {
    tb1[i].resize(q_max);
    tb2[i].resize(q_max);
  }
  for (int q1 = 1; q1 <= q_max; q1++) {
    for (int q2 = 1; q2 <= q_max; q2++) {
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
      int max_pr = 0;
      mpz_class max_ac = 0;
      int max_i = bit_length(min(q1, q2)) - 1 / 2;
      for (int i = 1; i <= max_i; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        auto[x, y, q3] = single_iter(n1, n2, i);
        auto finish = std::chrono::high_resolution_clock::now();
        auto t =
            std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start)
                .count();
        int pr = i * 1e9 / t;
        mpz_class ac = max(q1, q2) / (q3 != 0 ? q3 : 1);
        if (pr > max_pr) {
          tb1[q1 - 1][q2 - 1] = {x, y, i};
        }
        if (ac > max_ac) {
          tb2[q1 - 1][q2 - 1] = {x, y, i};
        }
      }
      cout << q1 << ' ' << q2 << ' ' << get<0>(tb1[q1 - 1][q2 - 1]) << ' '
           << get<1>(tb1[q1 - 1][q2 - 1]) << ' ' << get<2>(tb1[q1 - 1][q2 - 1])
           << ' ' << max_pr << get<0>(tb2[q1 - 1][q2 - 1]) << ' '
           << get<1>(tb2[q1 - 1][q2 - 1]) << ' ' << get<2>(tb2[q1 - 1][q2 - 1])
           << ' ' << max_ac << endl;
    }
  }
  return 0;
}
