import scipy
import numpy as np
from scipy import linalg, matrix
from random import randint
from numpy.polynomial import polynomial

def null(s):
    eps = 1e-12
    u, v, vh = scipy.linalg.svd(s)
    padding = max(0,np.shape(s)[1]-np.shape(v)[0])
    null_mask = np.concatenate(((v <= eps), np.ones((padding,),dtype=bool)),axis=0)
    null_space = scipy.compress(null_mask, vh, axis=0)
    return scipy.transpose(null_space)

m = 5
n = 5
dd = 3
f = [randint(0, 10) for _ in range(m + 1 - dd)]
g = [randint(0, 10) for _ in range(n + 1 - dd)]
if f[-1] == 0: f[-1] = 1
if g[-1] == 0: g[-1] = 1
d_exact = [randint(0, 10) for _ in range(dd + 1)]
if d_exact[-1] == 0: d_exact[-1] = 1
f = polynomial.polymul(f, d_exact)
g = polynomial.polymul(g, d_exact)
print(len(f), len(g))
#print("+".join("%s*x^%s" % (e, i) for (i, e) in enumerate(f)))
#print("+".join("%s*x^%s" % (e, i) for (i, e) in enumerate(g)))
s = np.zeros(shape=(m + n, m + n))
for i in range(m + n):
    for j in range(m + n):
        if j < n:
            s[i, j] = f[m + j - i] if 0 <= (m + j - i) <= m else 0
        else:
            s[i, j] = g[j - i] if 0 <= (j - i) <= n else 0
print(s)
deg_f = n
while f[deg_f] == 0: deg_f -= 1
deg_g = n
while g[deg_g] == 0: deg_g -= 1
r = np.linalg.matrix_rank(s)
deg_d = max(0, deg_f + deg_g - r)
print(deg_f, deg_g, deg_d)
f = np.sum(null(s[:r-1, :]), axis=1)
d = (s @ f)[-deg_d-1:]
d /= d[0]
d = d[::-1]
print(d_exact)
print(d)
