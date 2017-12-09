import matplotlib.pyplot
from functools import reduce

def product(s):
	return reduce(lambda x, y: x * y, s)

def witness_count(n):
    s, d = 0, n - 1
    while d % 2 == 0:
        s += 1
        d //= 2
    # n - 1 == (2 ** s) * d
    h = 0
    for a in range(1, n):
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            h += 1
            continue
        for r in range(s - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        if x == n - 1:
            h += 1
    return h


# Eratosthenes sieve
p = list(range(3, 3330, 2))
primes = []
while 1:
    np = [i for i in p if i % p[0]]
    primes.append(p[0])
    if not np:
        break
    p = np

print(primes)

primes1 = [p for p in primes]
primes2 = [p for p in primes if p < (10000 // (3 * 3))]
primes3 = [p for p in primes if p < (10000 // (3 * 3 * 3))]
primes4 = [p for p in primes if p < (10000 // (3 * 3 * 3 * 3))]

g1 = sorted({(p1 * p2, round(p1 * p2 * product(1 - 1/p for p in {p1, p2}))) 
			 for p1 in primes1 for p2 in primes1 
			 if p1 <= p2 and p1 * p2 < 10000}, key=lambda t: t[0])
g2 = sorted({(p1 * p2 * p3, round(p1 * p2 * p3 * product(1 - 1/p for p in {p1, p2, p3})))
             for p1 in primes2 for p2 in primes2 for p3 in primes2
			 if p1 <= p2 <= p3 and p1 * p2 * p3 < 10000}, key=lambda t: t[0])
g3 = sorted({(p1 * p2 * p3 * p4, round(p1 * p2 * p3 * p4 * product(1 - 1/p for p in {p1, p2, p3, p4}))) 
             for p1 in primes3 for p2 in primes3 for p3 in primes3 for p4 in primes3 
             if p1 <= p2 <= p3 <= p4 and p1 * p2 * p3 * p4 < 10000}, key=lambda t: t[0])
g4 = sorted({(p1 * p2 * p3 * p4 * p5, round(p1 * p2 * p3 * p4 * p5 * product(1 - 1/p for p in {p1, p2, p3, p4, p5}))) 
             for p1 in primes4 for p2 in primes4 for p3 in primes4 for p4 in primes4 for p5 in primes4
             if p1 <= p2 <= p3 <= p4 <= p5 and p1 * p2 * p3 * p4 * p5 < 10000}, key=lambda t: t[0])
			 
g5 = sorted({(p1 * p2, round(p1 * p2 * product(1 - 1/p for p in {p1, p2}))) 
			 for p1 in primes1 for p2 in primes1 
			 if p1 < p2 and p1 * p2 < 10000 and (p2 - p1) % ((p1 - 1) // 2) == 0}, key=lambda t: t[0])
			 
for g in [g1, g2, g3, g4, g5]:
    ns = [t[0] for t in g]
    phi = [t[1] for t in g]
    # print(g)
    witnesses = [witness_count(n) for n in ns]
    ratios = [witnesses[i] / phi[i] for i in range(len(ns))]
    matplotlib.pyplot.plot(ns, ratios, "r-")
    matplotlib.pyplot.show()
    ratios = [witnesses[i] / phi[i] for i in range(len(ns))]
    errors = [(ns[i], phi[i], witnesses[i]) for i in range(len(ns)) if witnesses[i] * 4 > phi[i]]
    print(max(ratios), sum(ratios) / len(ratios))
    print(errors)
    by_intv = [[ratios[i] for i in range(len(ratios)) if 1000 * j < ns[i] < 1000 * (j + 1)] for j in range(10)]
    ratios_intv = [sum(intv) / len(intv) for intv in by_intv]
    print(ratios_intv)