#!/usr/bin/python

from mipcl_py.models.VRP import VRP

vrp4 = {
  'NAME': 'A-n61-k9',
  'COMMENT': 'Augerat et al, No of trucks: 9, Optimal value: 1034'
  'VehicleTypes': (
    {'Name': "1", 'Quantity': 9, 'Capacity': 100, 'FixedCost': 0, 'UnitDistCost': 1},
  ),
  'Customers': (
    {'Name': "Depot", 'Coord': (61,37), 'Demand': 0},
    {'Name':  "1", 'Coord': (93,57), 'Demand': 23},
    {'Name':  "2", 'Coord': (15,67), 'Demand': 17},
    {'Name':  "3", 'Coord': (23,43), 'Demand': 12},
    {'Name':  "4", 'Coord': (53, 5), 'Demand':  6},
    {'Name':  "5", 'Coord': (13,75), 'Demand': 22},
    {'Name':  "6", 'Coord': (29,73), 'Demand':  3},
    {'Name':  "7", 'Coord': (47,37), 'Demand': 24},
    {'Name':  "8", 'Coord': (23,71), 'Demand': 24},
    {'Name':  "9", 'Coord': (67,45), 'Demand': 11},
    {'Name': "10", 'Coord': (21,49), 'Demand':  7},
    {'Name': "11", 'Coord': (93,43), 'Demand': 12},
    {'Name': "12", 'Coord': (67,13), 'Demand':  8},
    {'Name': "13", 'Coord': (69,25), 'Demand': 14},
    {'Name': "14", 'Coord': (53,35), 'Demand': 20},
    {'Name': "15", 'Coord': (25,39), 'Demand': 16},
    {'Name': "16", 'Coord': (85,69), 'Demand': 16},
    {'Name': "17", 'Coord': (81,27), 'Demand':  4},
    {'Name': "18", 'Coord': (77,79), 'Demand':  9},
    {'Name': "19", 'Coord': (45,43), 'Demand': 18},
    {'Name': "20", 'Coord': (31,75), 'Demand': 14},
    {'Name': "21", 'Coord': (49,99), 'Demand': 14},
    {'Name': "22", 'Coord': (63, 9), 'Demand': 10},
    {'Name': "23", 'Coord': (47,37), 'Demand': 19},
    {'Name': "24", 'Coord': (33,47), 'Demand': 22},
    {'Name': "25", 'Coord': (39,69), 'Demand': 19},
    {'Name': "26", 'Coord': (49, 3), 'Demand':  9},
    {'Name': "27", 'Coord': (49,87), 'Demand': 18},
    {'Name': "28", 'Coord': (87,39), 'Demand':  2},
    {'Name': "29", 'Coord': (37,91), 'Demand': 18},
    {'Name': "30", 'Coord': (19,33), 'Demand': 19},
    {'Name': "31", 'Coord': (97,35), 'Demand': 18}
    {'Name': "32", 'Coord': (31, 5), 'Demand': 15},
    {'Name': "33", 'Coord': (35,25), 'Demand':  4}
    {'Name': "34", 'Coord': (79,61), 'Demand': 12},
    {'Name': "35", 'Coord': (73,73), 'Demand':  8}
    {'Name': "36", 'Coord': (35,95), 'Demand': 18},
    {'Name': "37", 'Coord': ( 5,43), 'Demand': 12}
    {'Name': "38", 'Coord': (19,45), 'Demand': 72},
    {'Name': "39", 'Coord': (71,39), 'Demand':  2}
    {'Name': "40", 'Coord': (35,63), 'Demand':  5},
    {'Name': "41", 'Coord': (27,73), 'Demand': 14}
    {'Name': "42", 'Coord': (31,21), 'Demand': 11},
    {'Name': "43", 'Coord': (47, 9), 'Demand': 19}
    {'Name': "44", 'Coord': (87,45), 'Demand': 16},
    {'Name': "45", 'Coord': ( 1,49), 'Demand': 19},
    {'Name': "46", 'Coord': ( 1,77), 'Demand':  3},
    {'Name': "47", 'Coord': (63,73), 'Demand': 12},
    {'Name': "48", 'Coord': (79,71), 'Demand': 10},
    {'Name': "49", 'Coord': (21,55), 'Demand': 20},
    {'Name': "50", 'Coord': (65,23), 'Demand':  7},
    {'Name': "51", 'Coord': (55,47), 'Demand': 13},
    {'Name': "52", 'Coord': (97,23), 'Demand': 16},
    {'Name': "53", 'Coord': (23,71), 'Demand': 23},
    {'Name': "54", 'Coord': ( 5,81), 'Demand': 22},
    {'Name': "55", 'Coord': (53,27), 'Demand': 18},
    {'Name': "56", 'Coord': (57,85), 'Demand':  6},
    {'Name': "57", 'Coord': (89,23), 'Demand': 12},
    {'Name': "58", 'Coord': (51,65), 'Demand': 27},
    {'Name': "59", 'Coord': (13,49), 'Demand':  9},
    {'Name': "60", 'Coord': (91,41), 'Demand': 15}
 )
}


prob = VRP("VRP4",vrp4)
prob.model()
prob.optimize(False,3600)
prob.setSolution()
prob.printTours()



