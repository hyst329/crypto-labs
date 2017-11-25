#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision;

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

cpp_int divide(cpp_int u, cpp_int v, cpp_int s) {
	cpp_int d = u, m = 0;
}