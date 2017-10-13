import random


# RSA algorithm test
def is_probable_prime(n, k=7):
    if n < 12:
        return n in (2, 3, 5, 7, 11)
    elif n % 2 == 0:
        return False
    else:
        s, d = 0, n - 1
        while d & 1 == 0:
            s, d = s + 1, d // 2
        for a in random.sample(range(2, n - 2), min(n - 4, k)):
            x = pow(a, d, n)
            if x != 1 and x + 1 != n:
                for r in range(1, s):
                    x = pow(x, 2, n)
                    if x == 1:
                        return False  # composite for sure
                    elif x == n - 1:
                        a = 0  # so we know loop didn't continue to end
                        break  # could be strong liar, try another a
                if a:
                    return False  # composite if we reached end of this loop
        return True  # probably prime if reached end of outer loop


def xgcd(a, b):
    x0, x1, y0, y1 = 1, 0, 0, 1
    while b != 0:
        r, a, b = a // b, b, a % b
        x0, x1 = x1, x0 - r * x1
        y0, y1 = y1, y0 - r * y1
    return a, x0, y0

def generate_prime(bits):
    while 1:
        p = (2 ** (bits - 1) + random.randint(0, 2 ** (bits - 2)) * 2 + 1)
        if is_probable_prime(p):
            return p


p1 = generate_prime(16)
q1 = generate_prime(16)
n1 = p1 * q1
p2 = generate_prime(16)
q2 = generate_prime(16)
n2 = p2 * q2
phi1 = (p1 - 1) * (q1 - 1)
phi2 = (p2 - 1) * (q2 - 1)
while 1:
    e1 = random.randint(1, phi1 - 1)
    if xgcd(e1, phi1)[0] == 1:
        break
while 1:
    e2 = random.randint(1, phi2 - 1)
    if xgcd(e2, phi2)[0] == 1:
        break

d1 = xgcd(e1, phi1)[1]
if d1 < 0:
    d1 += phi1
d2 = xgcd(e2, phi2)[1]
if d2 < 0:
    d2 += phi2

print(f"p1 = {p1}, q1 = {q1}, n1 = {n1}, phi1 = {phi1}")
print(f"p2 = {p2}, q2 = {q2}, n2 = {n2}, phi2 = {phi2}")
print(f"e1 = {e1}, d1 = {d1}, e2 = {e2}, d2 = {d2}")
m = 28975723
c = pow(pow(m, d1, n1), e2, n2)
m0 = pow(pow(c, d2, n2), e1, n1) 
print(f"m = {m}, c = {c}, m0 = {m0}")