from sympy import symbols, pi, sqrt, atan2, cos, sin
from manifold import Manifold

manifold = Manifold()
x, y, z = manifold.x
r = symbols("r")
g = [[r, 0, 0], [0, r * cos(x)**2, 0], [0, 0, r * cos(x) ** 2 * cos(y) ** 2]]
# g = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
manifold.set_metric(g)

C = manifold.C
R = manifold.R

for i1 in range(3):
    for i2 in range(3):
        for i3 in range(3):
            if C[i1, i2, i3]:
                print('Γ^%s_%s%s:'%(i1, i2, i3), C[i1, i2, i3])

for i1 in range(3):
    for i2 in range(3):
        for i3 in range(3):
            for i4 in range(3):
                if R[i1, i2, i3, i4]:
                    print('R^%s_%s%s%s:'%(i1, i2, i3, i4), R[i1, i2, i3, i4])
