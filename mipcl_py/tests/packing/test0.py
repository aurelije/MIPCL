#!/usr/bin/python

from mipcl_py.models.multiPacking import Packing

instance = {
  'Cont. Sizes': (10, 10),
  'Items': (
     {'Type': 1, 'Sizes': (3, 7), 'Cost': 35},
     {'Type': 1, 'Sizes': (3, 7), 'Cost': 35},
     {'Type': 2, 'Sizes': (8, 2), 'Cost': 40},
     {'Type': 2, 'Sizes': (8, 2), 'Cost': 40},
     {'Type': 3, 'Sizes': (10, 2), 'Cost': 27},
     {'Type': 4, 'Sizes': (5, 4), 'Cost': 23},
     {'Type': 4, 'Sizes': (5, 4), 'Cost': 23},
     {'Type': 4, 'Sizes': (5, 4), 'Cost': 23},
     {'Type': 5, 'Sizes': (2, 9), 'Cost': 43},
     {'Type': 5, 'Sizes': (2, 9), 'Cost': 43}
  )
}

prob = Packing("test0",instance)
prob.model()
prob.optimize(False,3600)
prob.printLayout()

