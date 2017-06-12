#!/usr/bin/python

from mipcl_py.models.VRP import VRP

vrp0 = {
  'NAME': "vrp0",
  'VehicleTypes': (
    {'Name': "1", 'Quantity': 2, 'Capacity': 100, 'FixedCost': 0, 'UnitDistCost': 1},
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
    {'Name': "13", 'Coord': (84,25), 'Demand': 16}
 )
}

prob = VRP("VRP0",vrp0)
prob.model()
prob.optimize(False,3600)
prob.setSolution()
prob.printSolution()

