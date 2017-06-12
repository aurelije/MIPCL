#!/usr/bin/python

from mipcl_py.models.multiPacking import Packing

instance = {
  'Cont. Sizes': (250, 250),
  'Items': (
     {'Type':  1, 'Sizes': (120, 133), 'Cost': 15960},
     {'Type':  2, 'Sizes': (135, 186), 'Cost': 25110},
     {'Type':  3, 'Sizes': (86, 75), 'Cost': 6450},
     {'Type':  4, 'Sizes': (103, 73), 'Cost': 7519},
     {'Type':  5, 'Sizes': (66, 85), 'Cost': 5610},
     {'Type':  6, 'Sizes': (135, 97), 'Cost': 13095},
     {'Type':  7, 'Sizes': (91, 175), 'Cost': 15925},
     {'Type':  8, 'Sizes': (131, 176), 'Cost': 23056},
     {'Type':  9, 'Sizes': (71, 176), 'Cost': 12496},
     {'Type': 10, 'Sizes': (153, 72), 'Cost': 11016},
     {'Type': 11, 'Sizes': (87, 148), 'Cost': 12876},
     {'Type': 12, 'Sizes': (168, 107), 'Cost': 17976},
     {'Type': 13, 'Sizes': (118, 90), 'Cost': 10620},
     {'Type': 14, 'Sizes': (140, 109), 'Cost': 15260},
     {'Type': 15, 'Sizes': (132, 159), 'Cost': 20988},
     {'Type': 16, 'Sizes': (152, 93), 'Cost': 14136},
     {'Type': 17, 'Sizes': (135, 68), 'Cost': 9180},
     {'Type': 18, 'Sizes': (121, 158), 'Cost': 19118},
     {'Type': 19, 'Sizes': (68, 94), 'Cost': 6392},
     {'Type': 20, 'Sizes': (155, 76), 'Cost': 11780}
   )
}

prob = Packing("gcut2",instance)#,factor=2)#,up=False)
prob.model()
prob.optimize(False,3600)
prob.printLayout()

