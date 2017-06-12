#!/usr/bin/python

from mipcl_py.models.VRP import VRP

vrp2 = {
  'NAME': "A-n32-k5",
  'COMMENT': "Augerat et al, No of trucks: 5, Optimal value: 784",
  'VehicleTypes': (
    {'Name': "1", 'Quantity': 5, 'Capacity': 100, 'FixedCost': 0, 'UnitDistCost': 1},
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
    {'Name': "21", 'Coord': (98,14), 'Demand': 12},
    {'Name': "22", 'Coord': ( 5,42), 'Demand':  4},
    {'Name': "23", 'Coord': (42, 9), 'Demand':  8},
    {'Name': "24", 'Coord': (61,62), 'Demand': 24},
    {'Name': "25", 'Coord': ( 9,97), 'Demand': 24},
    {'Name': "26", 'Coord': (80,55), 'Demand':  2},
    {'Name': "27", 'Coord': (57,69), 'Demand': 20},
    {'Name': "28", 'Coord': (23,15), 'Demand': 15},
    {'Name': "29", 'Coord': (20,70), 'Demand':  2},
    {'Name': "30", 'Coord': (85,60), 'Demand': 14},
    {'Name': "31", 'Coord': (98, 5), 'Demand':  9}
 )
}

prob = VRP("VRP2",vrp2)
prob.model()
prob.optimize(False,3600)
prob.setSolution()
prob.printSolution()

