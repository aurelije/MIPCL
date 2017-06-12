#!/usr/bin/python
from mipcl_py.models.fl import Fl

q = 2
b = [10, 8, 20, 12, 14]
u = [40, 40, 40, 40, 40]
f = [0, 0, 0, 0, 0]
c = [
   [ 0, 11,  8, 12, 15],
   [11,  0, 10,  7, 13],
   [ 8, 10,  0,  9,  9],
   [12,  7,  9,  0,  6],
   [15, 13,  9,  6,  0]
]

prob = Fl("test1")
prob.model(q,b,u,f,c)
prob.optimize()
prob.printSolution()

