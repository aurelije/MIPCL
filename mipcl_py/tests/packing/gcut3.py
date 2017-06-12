#!/usr/bin/python

from mipcl_py.models.multiPacking import Packing

instance = {
  'Cont. Sizes': (250, 250),
  'Items': (
     {'Type':  1, 'Sizes': (66, 80), 'Cost': 5280},
     {'Type':  2, 'Sizes': (164, 107), 'Cost': 17548},
     {'Type':  3, 'Sizes': (64, 184), 'Cost': 11776},
     {'Type':  4, 'Sizes': (121, 86), 'Cost': 10406},
     {'Type':  5, 'Sizes': (163, 135), 'Cost': 22005},
     {'Type':  6, 'Sizes': (85, 98), 'Cost': 8330},
     {'Type':  7, 'Sizes': (81, 102), 'Cost': 8262},
     {'Type':  8, 'Sizes': (103, 186), 'Cost': 19158},
     {'Type':  9, 'Sizes': (152, 106), 'Cost': 16112},
     {'Type': 10, 'Sizes': (176, 139), 'Cost': 24464},
     {'Type': 11, 'Sizes': (111, 118), 'Cost': 13098},
     {'Type': 12, 'Sizes': (69, 169), 'Cost': 11661},
     {'Type': 13, 'Sizes': (146, 133), 'Cost': 19418},
     {'Type': 14, 'Sizes': (112, 112), 'Cost': 12544},
     {'Type': 15, 'Sizes': (133, 160), 'Cost': 21280},
     {'Type': 16, 'Sizes': (63, 129), 'Cost': 8127},
     {'Type': 17, 'Sizes': (163, 152), 'Cost': 24776},
     {'Type': 18, 'Sizes': (110, 155), 'Cost': 17050},
     {'Type': 19, 'Sizes': (96, 136), 'Cost': 13056},
     {'Type': 20, 'Sizes': (92, 142), 'Cost': 13064},
     {'Type': 21, 'Sizes': (84, 143), 'Cost': 12012},
     {'Type': 22, 'Sizes': (119, 133), 'Cost': 15827},
     {'Type': 23, 'Sizes': (71, 71), 'Cost': 5041},
     {'Type': 24, 'Sizes': (146, 84), 'Cost': 12264},
     {'Type': 25, 'Sizes': (93, 86), 'Cost': 7998},
     {'Type': 26, 'Sizes': (89, 86), 'Cost': 7654},
     {'Type': 27, 'Sizes': (101, 146), 'Cost': 14746},
     {'Type': 28, 'Sizes': (172, 73), 'Cost': 12556},
     {'Type': 29, 'Sizes': (73, 169), 'Cost': 12337},
     {'Type': 30, 'Sizes': (99, 99), 'Cost': 9801}
  )
}

prob = Packing("gcut3",instance,factor=10)#, up=False)
prob.model()
prob.optimize(False,3600)
prob.printLayout()

