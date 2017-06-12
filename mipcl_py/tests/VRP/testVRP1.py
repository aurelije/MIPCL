#!/usr/bin/python

from mipcl_py.models.VRP import VRP

vrp1 = {
  'NAME': "vrp1",
  'VehicleTypes': (
    {'Name': "1", 'Quantity': 3, 'Capacity': 100, 'FixedCost': 0, 'UnitDistCost': 1},
  ),
  'Customers': (
    {'Name': "Depot", 'Coord': (82,76), 'Demand': 0},
    {'Name':  "1", 'Coord': (96,44), 'Demand': 19},
    {'Name':  "2", 'Coord': (50, 5), 'Demand': 21},
    {'Name':  "3", 'Coord': (49, 8), 'Demand':  6},
    {'Name':  "4", 'Coord': (13, 7), 'Demand': 19},
    {'Name':  "5", 'Coord': (29,89), 'Demand':  7},
    {'Name':  "6", 'Coord': (58,30), 'Demand': 12},
    {'Name':  "7", 'Coord': (84,39), 'Demand': 16},
    {'Name':  "8", 'Coord': (14,24), 'Demand':  6},
    {'Name':  "9", 'Coord': ( 2,39), 'Demand': 16},
    {'Name': "10", 'Coord': ( 3,82), 'Demand':  8},
    {'Name': "11", 'Coord': ( 5,10), 'Demand': 14},
    {'Name': "12", 'Coord': (98,52), 'Demand': 21},
    {'Name': "13", 'Coord': (84,25), 'Demand': 16},
    {'Name': "14", 'Coord': (61,59), 'Demand':  3},
    {'Name': "15", 'Coord': ( 1,65), 'Demand': 22},
    {'Name': "16", 'Coord': (88,51), 'Demand': 18},
    {'Name': "17", 'Coord': (91, 2), 'Demand': 19},
    {'Name': "18", 'Coord': (19,32), 'Demand':  1},
    {'Name': "19", 'Coord': (93, 3), 'Demand': 24},
    {'Name': "20", 'Coord': (50,93), 'Demand':  8},
    {'Name': "21", 'Coord': (98,14), 'Demand': 12}
 )
}

prob = VRP("VRP1",vrp1)
prob.model()
prob.optimize(False,3600)
prob.setSolution()
prob.printSolution()

