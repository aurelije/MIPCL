#!/usr/bin/python

from mipcl_py.models.multiPacking import Packing

instance = {
  'Cont. Sizes': (500, 500),
  'Items': (
     {'Type':  1, 'Sizes': (313, 305), 'Cost': 95465},
     {'Type':  2, 'Sizes': (292, 222), 'Cost': 64824},
     {'Type':  3, 'Sizes': (330, 253), 'Cost': 83490},
     {'Type':  4, 'Sizes': (212, 256), 'Cost': 54272},
     {'Type':  5, 'Sizes': (132, 189), 'Cost': 24948},
     {'Type':  6, 'Sizes': (149, 296), 'Cost': 44104},
     {'Type':  7, 'Sizes': (294, 137), 'Cost': 40278},
     {'Type':  8, 'Sizes': (225, 345), 'Cost': 77625},
     {'Type':  9, 'Sizes': (345, 220), 'Cost': 75900},
     {'Type': 10, 'Sizes': (337, 177), 'Cost': 59649},
     {'Type': 11, 'Sizes': (189, 300), 'Cost': 56700},
     {'Type': 12, 'Sizes': (234, 321), 'Cost': 75114},
     {'Type': 13, 'Sizes': (335, 272), 'Cost': 91120},
     {'Type': 14, 'Sizes': (354, 244), 'Cost': 86376},
     {'Type': 15, 'Sizes': (149, 169), 'Cost': 25181},
     {'Type': 16, 'Sizes': (355, 165), 'Cost': 58575},
     {'Type': 17, 'Sizes': (260, 220), 'Cost': 57200},
     {'Type': 18, 'Sizes': (210, 241), 'Cost': 50610},
     {'Type': 19, 'Sizes': (194, 372), 'Cost': 72168},
     {'Type': 20, 'Sizes': (287, 134), 'Cost': 38458}
  )
}

prob = Packing("gcut6",instance,factor=10)#, up=False)
prob.model()
prob.optimize(False,3600)
prob.printLayout()

