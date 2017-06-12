#!/usr/bin/python
from mipcl_py.models.unitcom import Unitcom

q = 1.2
d = [50, 60, 50, 100, 120, 90, 90, 100, 90, 120, 110, 70]
units = [
    [ 2, 12, 12, 12, 100,  1, 10],
    [ 2, 12, 12, 12, 100,  1, 10],
    [ 5, 35, 35, 35, 300,  5,  4],
    [20, 50, 50, 50, 400, 10,  3],
    [40, 75, 15, 20, 800, 15,  2]
]

prob = Unitcom("test1")
prob.model(q,d,units)
prob.optimize(False)
prob.printSolution()

