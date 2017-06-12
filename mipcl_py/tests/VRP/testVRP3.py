#!/usr/bin/python

from mipcl_py.models.VRP import VRP

vrp3 = {
  'NAME': 'A-n48-k7',
  'COMMENT': 'Augerat et al, No of trucks: 7, Optimal value: 1073'
  'VehicleTypes': (
    {'Name': "1", 'Quantity': 7, 'Capacity': 100, 'FixedCost': 0, 'UnitDistCost': 1},
  ),
  ),
  'Customers': (
    {'Name': "Depot", 'Coord': (47, 5), 'Demand': 0},
    {'Name':  "1", 'Coord': ( 1,19), 'Demand': 20},
    {'Name':  "2", 'Coord': (97,35), 'Demand': 14},
    {'Name':  "3", 'Coord': (23,79), 'Demand':  5},
    {'Name':  "4", 'Coord': (77,87), 'Demand': 11},
    {'Name':  "5", 'Coord': ( 3, 9), 'Demand': 22},
    {'Name':  "6", 'Coord': ( 5,27), 'Demand': 25},
    {'Name':  "7", 'Coord': (41,53), 'Demand':  2},
    {'Name':  "8", 'Coord': (51,87), 'Demand': 18},
    {'Name':  "9", 'Coord': (67,73), 'Demand': 10},
    {'Name': "10", 'Coord': (89,45), 'Demand': 26},
    {'Name': "11", 'Coord': (71,99), 'Demand': 14},
    {'Name': "12", 'Coord': (11, 1), 'Demand': 22},
    {'Name': "13", 'Coord': (85,85), 'Demand':  9},
    {'Name': "14", 'Coord': (57,11), 'Demand': 11},
    {'Name': "15", 'Coord': (57,85), 'Demand': 18},
    {'Name': "16", 'Coord': (71,33), 'Demand': 24},
    {'Name': "17", 'Coord': (61,13), 'Demand': 15},
    {'Name': "18", 'Coord': (39,15), 'Demand': 23},
    {'Name': "19", 'Coord': (13,59), 'Demand': 16},
    {'Name': "20", 'Coord': (43,99), 'Demand': 14},
    {'Name': "21", 'Coord': (87,73), 'Demand':  8},
    {'Name': "22", 'Coord': (11,37), 'Demand':  5},
    {'Name': "23", 'Coord': (21,11), 'Demand': 12},
    {'Name': "24", 'Coord': (77,81), 'Demand':  8},
    {'Name': "25", 'Coord': ( 3,63), 'Demand': 16},
    {'Name': "26", 'Coord': (47,95), 'Demand': 12},
    {'Name': "27", 'Coord': (53,75), 'Demand': 15},
    {'Name': "28", 'Coord': (73,55), 'Demand':  9},
    {'Name': "29", 'Coord': (81,71), 'Demand':  2},
    {'Name': "30", 'Coord': (89,75), 'Demand': 10},
    {'Name': "31", 'Coord': (11, 9), 'Demand':  2},
    {'Name': "32", 'Coord': (89,75), 'Demand':  3},
    {'Name': "33", 'Coord': (11, 9), 'Demand': 20},
    {'Name': "34", 'Coord': (27,37), 'Demand':  3},
    {'Name': "35", 'Coord': (95,59), 'Demand': 13},
    {'Name': "36", 'Coord': (63,63), 'Demand': 25},
    {'Name': "37", 'Coord': (37,21), 'Demand': 23},
    {'Name': "38", 'Coord': (33,47), 'Demand':  8},
    {'Name': "39", 'Coord': (23,63), 'Demand': 16},
    {'Name': "40", 'Coord': (45,43), 'Demand':  9},
    {'Name': "41", 'Coord': (83, 7), 'Demand': 14},
    {'Name': "42", 'Coord': (69,91), 'Demand':  4},
    {'Name': "43", 'Coord': (13,11), 'Demand': 13},
    {'Name': "44", 'Coord': (37,15), 'Demand':  7},
    {'Name': "45", 'Coord': (53,59), 'Demand': 16},
    {'Name': "46", 'Coord': (97,83), 'Demand': 18},
    {'Name': "47", 'Coord': (75,31), 'Demand': 16}
 )
}


prob = VRP("VRP3",vrp3)
prob.model()
prob.optimize(False,3600)
prob.setSolution()
prob.printTours()

