#!/usr/bin/python

from mipcl_py.models.multiPacking import Packing
   
instance = {
  'Cont. Sizes': (3000, 3000),
  'Items': (
     {'Type':  1, 'Sizes': (365, 185), 'Cost': 67525},
     {'Type':  2, 'Sizes': (378, 200), 'Cost': 75600},
     {'Type':  3, 'Sizes': (410, 165), 'Cost': 67650},
     {'Type':  4, 'Sizes': (425, 148), 'Cost': 62900},
     {'Type':  5, 'Sizes': (425, 296), 'Cost': 125800},
     {'Type':  6, 'Sizes': (439, 116), 'Cost': 50924},
     {'Type':  7, 'Sizes': (464, 1006), 'Cost': 466784},
     {'Type':  8, 'Sizes': (520, 205), 'Cost': 106600},
     {'Type':  9, 'Sizes': (520, 350), 'Cost': 182000},
     {'Type': 10, 'Sizes': (540, 530), 'Cost': 286200},
     {'Type': 11, 'Sizes': (549, 1413), 'Cost': 775737},
     {'Type': 12, 'Sizes': (549, 1882), 'Cost': 1033218},
     {'Type': 13, 'Sizes': (553, 496), 'Cost': 274288},
     {'Type': 14, 'Sizes': (555, 755), 'Cost': 419025},
     {'Type': 15, 'Sizes': (555, 496), 'Cost': 275280},
     {'Type': 16, 'Sizes': (555, 659), 'Cost': 365745},
     {'Type': 17, 'Sizes': (567, 473), 'Cost': 268191},
     {'Type': 18, 'Sizes': (572, 592), 'Cost': 338624},
     {'Type': 19, 'Sizes': (572, 975), 'Cost': 557700},
     {'Type': 20, 'Sizes': (572, 1175), 'Cost': 672100},
     {'Type': 21, 'Sizes': (572, 1575), 'Cost': 900900},
     {'Type': 22, 'Sizes': (572, 1390), 'Cost': 795080},
     {'Type': 23, 'Sizes': (572, 1490), 'Cost': 852280},
     {'Type': 24, 'Sizes': (572, 1590), 'Cost': 909480},
     {'Type': 25, 'Sizes': (572, 1690), 'Cost': 966680},
     {'Type': 26, 'Sizes': (572, 1890), 'Cost': 1081080},
     {'Type': 27, 'Sizes': (610, 625), 'Cost': 381250},
     {'Type': 28, 'Sizes': (660, 490), 'Cost': 323400},
     {'Type': 29, 'Sizes': (690, 447), 'Cost': 308430},
     {'Type': 30, 'Sizes': (949, 445), 'Cost': 422305},
     {'Type': 31, 'Sizes': (949, 478), 'Cost': 453622},
     {'Type': 32, 'Sizes': (970, 463), 'Cost': 449110}
  )
}

prob = Packing("gcut13",instance, factor=10)#,up=False)
prob.model()
prob.optimize(False,3600)
prob.printLayout()

