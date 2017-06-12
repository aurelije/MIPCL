#!/usr/bin/python

from mipcl_py.models.multiPacking import Packing

instance = {
  'Cont. Sizes': (250, 250),
  'Items': (
     {'Type':  1, 'Sizes': (167, 184), 'Cost': 30728},
     {'Type':  2, 'Sizes': (114, 118), 'Cost': 13452},
     {'Type':  3, 'Sizes': (167, 152), 'Cost': 25384},
     {'Type':  4, 'Sizes': (83, 140),  'Cost': 11620},
     {'Type':  5, 'Sizes': (70, 86),   'Cost': 6020},
     {'Type':  6, 'Sizes': (143, 166), 'Cost': 23738},
     {'Type':  7, 'Sizes': (120, 160), 'Cost': 19200},
     {'Type':  8, 'Sizes': (66, 148),  'Cost': 9768},
     {'Type':  9, 'Sizes': (87, 141),  'Cost': 12267},
     {'Type': 10, 'Sizes': (69, 165),  'Cost': 11385}
  )
}

prob = Packing("gcut1",instance)
prob.model()
prob.optimize(False,3600)
prob.printLayout()

