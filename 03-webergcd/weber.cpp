#include <boost/multiprecision/cpp_int.hpp>
#include <map>
#include <iostream>
#include <boost/random.hpp>
#include <boost/chrono.hpp>

using namespace boost::multiprecision;
using namespace boost::random;
using namespace std;
using namespace boost::chrono;

int bit_length(cpp_int x) {
	if (x < 0) x = -x;
	int r = 0;
	while (x > 0) {
		x >>= 1;
		r += 1;
	}
	return r;
}

cpp_int gcd(cpp_int x, cpp_int y) {
	if (x < 0) x = -x;
	if (y < 0) y = -y;
	while (x && y) {
		if (x > y)
			x = x % y;
		else
			y = y % x;
	}
	return x + y;
}

cpp_int divide(cpp_int u, cpp_int v, int s) {
	cpp_int d = u, m = 0;
	for (int i = 0; i < s; i++) {
		if (d % (cpp_int(1) << (i + 1))) {
				d -= (cpp_int(1) << i) * v;
				m += (cpp_int(1) << i);
		}
	}
	return m;
}

pair<cpp_int, cpp_int> reduced_rat_mod(cpp_int x, cpp_int y, int h) {
	cpp_int c = divide(x, y, 2 * h);
	pair<cpp_int, cpp_int> f1 = {cpp_int(1) << (2 * h), 0};
	pair<cpp_int, cpp_int> f2 = {c, 1};
	while (f2.first >= cpp_int(1) << h) {
		f1 = {f1.first - (f1.first / f2.first) * f2.first, f1.second - (f1.first / f2.first) * f2.second};
		swap(f1, f2);
	}
	return f2;
}

cpp_int dmod(cpp_int u0, cpp_int v0, int w) {
	cpp_int u = u0;
	while (bit_length(u) >= bit_length(v0) + w) {
		if (u % (cpp_int(1) << w)) {
			u = abs(u - divide(u, v0, w) * v0);
			u >>= w;
		}
	}
	int d = bit_length(u) - bit_length(v0);
	if (u % (cpp_int(1) << max(d + 1, 0))) {
		u = abs(u - divide(u, v0, d + 1) * v0);
	}
	return u >> max(d + 1, 0);
}

cpp_int weber_gcd(cpp_int u0, cpp_int v0, int w) {
	if (u0 < v0) swap(u0, v0);
	auto s = [](cpp_int v) { return 64; };
	auto t = [](cpp_int v) { return 192; };
	cpp_int u = u0, v = v0;
	while (v != 0) {
		//cout << u << " " << v << endl;
		if (bit_length(u) - bit_length(v) > s(v)) {
			u = dmod(u, v, w);
		} else {
			auto p = reduced_rat_mod(u, v, t(v));
			cpp_int n = p.first, d = p.second;
			u = abs(n * v - d * u) >> (2 * t(v));
		}
		while ((u % 2 == 0) && u) {
			u /= 2;
		}
		swap(u, v);
	}
	cpp_int x = gcd(dmod(v0, u, w), u);
  return gcd(dmod(u0, x, w), x);
}

int main() {
	typedef independent_bits_engine<mt19937, 256, cpp_int> generator_type;
  generator_type gen;
	gen.seed((std::time(0)));
	const int n = 1000;
	cpp_int a1[n], a2[n], w[n], e[n];
	for (int i = 0; i < n; i++) {
		a1[i] = gen();
		a2[i] = gen();
	}
	steady_clock::time_point start = steady_clock::now();
	for (int i = 0; i < n; i++) {
		cpp_int q1 = a1[i], q2 = a2[i];
		cpp_int n1 = q1, n2 = q2;
		int p1 = 0, p2 = 0;
		while(n1 % 2 == 0) {
			n1 >>= 1;
			p1++;
		}
		while(n2 % 2 == 0) {
			n2 >>= 1;
			p2++;
		}
		///cout << q1 << ' ' << q2 << endl;
		cpp_int g = weber_gcd(n1, n2, 64) * (cpp_int(1) << min(p1, p2));
		//cout << g << endl;
		w[i] = g;
	}
	duration<double> d = steady_clock::now() - start;
	cout << "Weber GCD: \t" << d.count() << endl;
	start = steady_clock::now();
	for (int i = 0; i < n; i++) {
		cpp_int q1 = a1[i], q2 = a2[i];
		///cout << q1 << ' ' << q2 << endl;
		cpp_int g = gcd(q1, q2);
		//cout << g << endl;
		e[i] = g;
	}
	d = steady_clock::now() - start;
	cout << "Euclidean GCD: \t" << d.count() << endl;
	for (int i = 0; i < n; i++) {
		cout << w[i] << " " << e[i] << endl;
		if (w[i] != e[i]) cout << "Error detected at position " << i << endl;
	}
}
