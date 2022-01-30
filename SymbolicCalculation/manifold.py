from sympy import symbols, pi, sqrt, atan2, cos, sin, latex, simplify
from sympy.diffgeom import Patch, CoordSystem
from sympy.diffgeom import Manifold as MF
from sympy.diffgeom import TensorProduct as TP
from sympy.diffgeom import metric_to_Riemann_components as Riemann
from sympy.diffgeom import metric_to_Christoffel_2nd
import sympy

class Manifold:
	def __init__(self):
		m = self.m = MF("M", 3)
		p = self.p = Patch('P', m)
		coord = self.coord = CoordSystem('c', p, symbols('x y z', real=True))
		self.x = coord.coord_functions()
		self.dx = coord.base_oneforms()
		self.e = coord.base_vectors()
		self.g = None
		self.metric = None
		self.C = None
		self.R = None
	def set_metric(self, g):
		c = ['x', 'y', 'z']
		self.g = g
		dx = self.dx
		metric = self.metric = sum([g[i][j] * TP(dx[i], dx[j]) for i in range(3) for j in range(3)])
		self.C = metric_to_Christoffel_2nd(metric)
		self.R = Riemann(metric)
		e = self.e
		A = []
		B = []
		V = []
		theta = symbols("theta")
		for i in range(0, 3):
			A.append(symbols("A.%s" % c[i]))
			B.append(symbols("B.%s" % c[i]))
			V.append(symbols("V.%s" % c[i]))
		# V2 = sum([g[i][j] * V[i] * V[j] for i in range(3) for j in range(3)])
		# for i in range(3):
		# V[i] /= len_v
		alpha = 0
		beta = 0
		for i in range(0, 3):
			for j in range(0, 3):
				alpha += g[i][j] * A[i] * V[i]
				beta += g[i][j] * B[i] * V[i]
		# rho2=alpha **2 + beta **2
		# alpha = alpha
		# beta = beta
		V_vertical = []
		for i in range(0, 3):
			V_vertical.append(V[i] - alpha * A[i] - beta * B[i])
		alpha_ = cos(theta) * alpha - sin(theta) * beta
		beta_ = sin(theta) * alpha + cos(theta) * beta
		V_ = []
		for i in range(0, 3):
			V_.append(alpha_ * A[i] + beta_ * B[i] + V_vertical[i])
			print(V_[i])
		# print(latex(V_[i].subs([(A[0], 1), (A[1], 0), (A[2], 0), (B[0], 0), (B[1], 1), (B[2], 0)])))
		# print("=====")
		self.rotate_V = V_
	def C(self, i, j, k, x, y, z):
		return self.C[i, j, k].subs((self.x[0], x), (self.x[1], y), (self.x[2], z))
	def R(self, i, j, k, l, x, y, z):
		return self.R[i, j, k, l].subs((self.x[0], x), (self.x[1], y), (self.x[2], z))
# def rotate(self, x, y, z, A, B, theta, V):
