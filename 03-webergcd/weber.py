from math import gcd
from secrets import randbelow

def divide(u, v, s):  # u / v mod 2^s
    d = u
    m = 0
    for i in range(s):
        if d % (2 ** (i + 1)):
            d -= (2 ** i) * v
            m += 2 ** i
    return m


def weber_gcd(u0, v0, w):
    def s(v): return 8

    def t(v): return 13

    u = u0
    v = v0
    while v != 0:
        # print (u, v)
        if u.bit_length() - v.bit_length() > s(v):
            u = dmod(u, v, w)
        else:
            n, d = reduced_rat_mod(u, v, t(v))
            u = abs(n * v - d * u) // (2 ** (2 * t(v)))
        while u % 2 == 0 and u != 0:
            u //= 2
        u, v = v, u
    x = gcd(dmod(v0, u, w), u)
    return gcd(dmod(u0, x, w), x)


def dmod(u0, v0, w):
    u = u0
    while u.bit_length() >= v0.bit_length() + w:
        if u % (2 ** w):
            u = abs(u - divide(u, v0, w) * v0)
            u //= 2 ** w
    d = u.bit_length() - v0.bit_length()
    if u % (2 ** (d + 1)):
        u = abs(u - divide(u, v0, d + 1) * v0)
    return u // 2 ** (d + 1)


def reduced_rat_mod(x, y, h):  # m = 2 ^ 2h
    c = divide(x, y, 2 * h)
    f1 = (2 ** (2 * h), 0)
    f2 = (c, 1)
    while f2[0] >= (2 ** h):
        f1 = (f1[0] - (f1[0] // f2[0]) * f2[0], f1[1] - (f1[0] // f2[0]) * f2[1])
        f1, f2 = f2, f1
    return f2

if __name__ == '__main__':
    for i in range(1000):
        q1 = randbelow(2 ** 128)
        q2 = randbelow(2 ** 128)
        if (q1 < q2):
            q1, q2 = q2, q1
        n1 = q1
        n2 = q2
        p1 = 0
        p2 = 0
        while n1 % 2 == 0:
            n1 //= 2
            p1 += 1
        while n2 % 2 == 0:
            n2 //= 2
            p2 += 1
        g = weber_gcd(n1, n2, 64) * (2 ** min(p1, p2))
        h = gcd(q1, q2)
        print(q1, q2)
        print(g, h)
        assert g == h
