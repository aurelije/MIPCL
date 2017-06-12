#!/usr/bin/python
from mipcl_py.models.transport import Transport

c = [
  [2.0, 3.12, 1.26, 4.12],
  [1.0, 9.12, 7.32, 8.25],
  [6.4, 5.10, 2.45, 9.39]
]
a = [26, 14, 33]
b = [25, 10, 6, 15]


prob = Transport("test1")
prob.model(a,b,c)
prob.optimize()
prob.printSolution()

