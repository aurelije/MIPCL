#!/usr/bin/python
from mipcl_py.models.projectScheduling import projectSchedule

project = {
    'Name': "j6048_10",
    'Horizon': 167,
    'renRes': (
        {'Name': "1", 'Quantity': 41},
        {'Name': "2", 'Quantity': 44},
        {'Name': "3", 'Quantity': 56},
        {'Name': "4", 'Quantity': 52}
    ),
    'nonRenRes': (),
    'Jobs': (
        {
            'Name': "0",
            'Modes': ({'Duration': 1, 'renRes': (3, 5, 3, 1)},),
            'Prec': ()
        },
        {
            'Name': "1",
            'Modes': ({'Duration': 9, 'renRes': (5, 1, 9, 1)},),
            'Prec': ()
        },
        {
            'Name': "2",
            'Modes': ({'Duration': 3, 'renRes': (2, 2, 1, 9)},),
            'Prec': ()
        },
        {
            'Name': "3",
            'Modes': ({'Duration': 6, 'renRes': (1, 1, 2, 6)},),
            'Prec': (0,)
        },
        {
            'Name': "4",
            'Modes': ({'Duration': 7, 'renRes': (4, 10, 10, 10)},),
            'Prec': (0,)
        },
        {
            'Name': "5",
            'Modes': ({'Duration': 4, 'renRes': (4, 2, 5, 9)},),
            'Prec': (0,)
        },
        {
            'Name': "6",
            'Modes': ({'Duration': 1, 'renRes': (5, 4, 1, 9)},),
            'Prec': (3,)
        },
        {
            'Name': "7",
            'Modes': ({'Duration': 3, 'renRes': (4, 8, 5, 6)},),
            'Prec': (4,)
        },
        {
            'Name': "8",
            'Modes': ({'Duration': 4, 'renRes': (8, 3, 9, 5)},),
            'Prec': (3,)
        },
        {
            'Name': "9",
            'Modes': ({'Duration': 2, 'renRes': (6, 3, 10, 1)},),
            'Prec': (3,)
        },
        {
            'Name': "10",
            'Modes': ({'Duration': 10, 'renRes': (8, 2, 2, 1)},),
            'Prec': (7,)
        },
        {
            'Name': "11",
            'Modes': ({'Duration': 2, 'renRes': (4, 4, 2, 8)},),
            'Prec': (9,)
        },
        {
            'Name': "12",
            'Modes': ({'Duration': 1, 'renRes': (7, 8, 3, 2)},),
            'Prec': (10,)
        },
        {
            'Name': "13",
            'Modes': ({'Duration': 5, 'renRes': (6, 2, 7, 10)},),
            'Prec': (6, 9)
        },
        {
            'Name': "14",
            'Modes': ({'Duration': 4, 'renRes': (3, 8, 3, 2)},),
            'Prec': (10,)
        },
        {
            'Name': "15",
            'Modes': ({'Duration': 4, 'renRes': (10, 9, 10, 9)},),
            'Prec': (8, 10)
        },
        {
            'Name': "16",
            'Modes': ({'Duration': 10, 'renRes': (2, 3, 10, 8)},),
            'Prec': (4,)
        },
        {
            'Name': "17",
            'Modes': ({'Duration': 9, 'renRes': (1, 3, 10, 10)},),
            'Prec': (5,)
        },
        {
            'Name': "18",
            'Modes': ({'Duration': 8, 'renRes': (1, 8, 5, 5)},),
            'Prec': (11,)
        },
        {
            'Name': "19",
            'Modes': ({'Duration': 2, 'renRes': (10, 8, 8, 9)},),
            'Prec': (17,)
        },
        {
            'Name': "20",
            'Modes': ({'Duration': 10, 'renRes': (1, 6, 3, 5)},),
            'Prec': (2,)
        },
        {
            'Name': "21",
            'Modes': ({'Duration': 7, 'renRes': (8, 3, 2, 9)},),
            'Prec': (12,)
        },
        {
            'Name': "22",
            'Modes': ({'Duration': 4, 'renRes': (9, 7, 1, 5)},),
            'Prec': (12,)
        },
        {
            'Name': "23",
            'Modes': ({'Duration': 1, 'renRes': (1, 8, 6, 4)},),
            'Prec': (15,)
        },
        {
            'Name': "24",
            'Modes': ({'Duration': 3, 'renRes': (7, 3, 1, 7)},),
            'Prec': (11,)
        },
        {
            'Name': "25",
            'Modes': ({'Duration': 8, 'renRes': (9, 5, 3, 9)},),
            'Prec': (11, 16, 20)
        },
        {
            'Name': "26",
            'Modes': ({'Duration': 4, 'renRes': (5, 6, 2, 6)},),
            'Prec': (7, 13)
        },
        {
            'Name': "27",
            'Modes': ({'Duration': 2, 'renRes': (1, 4, 3, 4)},),
            'Prec': (14, 15, 21)
        },
        {
            'Name': "28",
            'Modes': ({'Duration': 3, 'renRes': (1, 1, 1, 9)},),
            'Prec': (6, 12, 18)
        },
        {
            'Name': "29",
            'Modes': ({'Duration': 6, 'renRes': (8, 1, 10, 3)},),
            'Prec': (2, 7, 17)
        },
        {
            'Name': "30",
            'Modes': ({'Duration': 3, 'renRes': (10, 2, 5, 1)},),
            'Prec': (19, 22, 24)
        },
        {
            'Name': "31",
            'Modes': ({'Duration': 4, 'renRes': (2, 10, 8, 4)},),
            'Prec': (2, 27, 28)
        },
        {
            'Name': "32",
            'Modes': ({'Duration': 10, 'renRes': (4, 3, 6, 10)},),
            'Prec': (15, 20, 30)
        },
        {
            'Name': "33",
            'Modes': ({'Duration': 3, 'renRes': (9, 6, 9, 9)},),
            'Prec': (14, 21, 22)
        },
        {
            'Name': "34",
            'Modes': ({'Duration': 4, 'renRes': (4, 9, 6, 6)},),
            'Prec': (8, 22)
        },
        {
            'Name': "35",
            'Modes': ({'Duration': 10, 'renRes': (6, 2, 7, 5)},),
            'Prec': (14, 25, 26)
        },
        {
            'Name': "36",
            'Modes': ({'Duration': 3, 'renRes': (3, 10, 10, 1)},),
            'Prec': (21, 25, 29)
        },
        {
            'Name': "37",
            'Modes': ({'Duration': 5, 'renRes': (6, 10, 5, 2)},),
            'Prec': (24, 26, 27)
        },
        {
            'Name': "38",
            'Modes': ({'Duration': 6, 'renRes': (7, 4, 7, 3)},),
            'Prec': (28, 29, 33)
        },
        {
            'Name': "39",
            'Modes': ({'Duration': 7, 'renRes': (7, 10, 9, 6)},),
            'Prec': (5, 8, 20)
        },
        {
            'Name': "40",
            'Modes': ({'Duration': 9, 'renRes': (4, 1, 5, 6)},),
            'Prec': (34, 35)
        },
        {
            'Name': "41",
            'Modes': ({'Duration': 3, 'renRes': (4, 6, 1, 3)},),
            'Prec': (16, 37, 38)
        },
        {
            'Name': "42",
            'Modes': ({'Duration': 2, 'renRes': (10, 3, 6, 5)},),
            'Prec': (9, 23,34)
        },
        {
            'Name': "43",
            'Modes': ({'Duration': 2, 'renRes': (7, 4, 4, 10)},),
            'Prec': (37, 39, 42)
        },
        {
            'Name': "44",
            'Modes': ({'Duration': 4, 'renRes': (4, 10, 8, 9)},),
            'Prec': (31, 36, 38)
        },
        {
            'Name': "45",
            'Modes': ({'Duration': 8, 'renRes': (9, 1, 4, 6)},),
            'Prec': (6, 29, 42)
        },
        {
            'Name': "46",
            'Modes': ({'Duration': 4, 'renRes': (10, 5, 7, 6)},),
            'Prec': (1, 40, 43)
        },
        {
            'Name': "47",
            'Modes': ({'Duration': 4, 'renRes': (10, 6, 7, 2)},),
            'Prec': (1, 32, 38)
        },
        {
            'Name': "48",
            'Modes': ({'Duration': 2, 'renRes': (9, 4, 5, 4)},),
            'Prec': (4, 19)
        },
        {
            'Name': "49",
            'Modes': ({'Duration': 2, 'renRes': (10, 5, 8, 1)},),
            'Prec': (18, 23, 32)
        },
        {
            'Name': "50",
            'Modes': ({'Duration': 9, 'renRes': (1, 7, 8, 4)},),
            'Prec': (23, 39, 44)
        },
        {
            'Name': "51",
            'Modes': ({'Duration': 8, 'renRes': (10, 5, 1, 2)},),
            'Prec': (19, 43, 50)
        },
        {
            'Name': "52",
            'Modes': ({'Duration': 2, 'renRes': (1, 9, 8, 8)},),
            'Prec': (1, 24, 40)
        },
        {
            'Name': "53",
            'Modes': ({'Duration': 10, 'renRes': (3, 7, 2, 10)},),
            'Prec': (32, 42, 47)
        },
        {
            'Name': "54",
            'Modes': ({'Duration': 4, 'renRes': (10, 10, 1, 6)},),
            'Prec': (27, 39, 40)
        },
        {
            'Name': "55",
            'Modes': ({'Duration': 10, 'renRes': (10, 3, 4, 4)},),
            'Prec': (35, 48, 53)
        },
        {
            'Name': "56",
            'Modes': ({'Duration': 10, 'renRes': (10, 2, 3, 7)},),
            'Prec': (41, 45, 49)
        },
        {
            'Name': "57",
            'Modes': ({'Duration': 8, 'renRes': (6, 5, 7, 6)},),
            'Prec': (51, 55, 56)
        },
        {
            'Name': "58",
            'Modes': ({'Duration': 4, 'renRes': (5, 5, 1, 5)},),
            'Prec': (5, 28, 52)
        },
        {
            'Name': "59",
            'Modes': ({'Duration': 8, 'renRes': (7, 8, 9, 1)},),
            'Prec': (17, 46, 54)
        }
    )
}


prob = projectSchedule(project)
prob.model()
prob.optimize(False,3600)
schedule = prob.getSchedule()
if schedule is not None:
    prob.printSchedule(schedule)
else:
    print('No schedule has been found')


