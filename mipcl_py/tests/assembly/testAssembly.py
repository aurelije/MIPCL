#!/usr/bin/python

from mipcl_py.models.assembly import Assembly

m = 9
C = 12

operations = {
    0: (6, ()),
    1: (7, (0,)),
    2: (5, ()),
    3: (3, (2,)),
    4: (2, ()),
    5: (5, (3,)),
    6: (6, (4,)),
    7: (3, ()),
    8: (4, ()),
    9: (3, ()),
   10: (4, (4, 6)),
   11: (3, (5,)),
   12: (5, (9,)),
   13: (2, (6,)),
   14: (6, ()),
   15: (8, (14,)),
   16: (3, (3, 7))
}

prob = Assembly("test1")
prob.model(m,C,operations)
prob.optimize(False)
prob.printSolution()

