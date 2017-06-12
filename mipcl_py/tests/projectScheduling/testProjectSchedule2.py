#!/usr/bin/python
from mipcl_py.models.projectScheduling import projectSchedule

project = {
    'Name': "j3048_10",
    'Horizon': 167,
    'renRes': (
        {'Name': "1", 'Quantity': 43},
        {'Name': "2", 'Quantity': 40},
        {'Name': "3", 'Quantity': 44},
        {'Name': "4", 'Quantity': 35}
    ),
    'nonRenRes': (),
    'Jobs': (
        {
            'Name': "0",
            'Modes': ({'Duration': 7, 'renRes': (3, 10, 4, 4)},),
            'Prec': ()
        },
        {
            'Name': "1",
            'Modes': ({'Duration': 10, 'renRes': (5, 8, 9, 8)},),
            'Prec': ()
        },
        {
            'Name': "2",
            'Modes': ({'Duration': 2, 'renRes': (4, 4, 3, 9)},),
            'Prec': ()
        },
        {
            'Name': "3",
            'Modes': ({'Duration': 6, 'renRes': (7, 5, 9, 10)},),
            'Prec': (0,)
        },
        {
            'Name': "4",
            'Modes': ({'Duration': 8, 'renRes': (4, 7, 3, 4)},),
            'Prec': (3,)
        },
        {
            'Name': "5",
            'Modes': ({'Duration': 1, 'renRes': (2, 6, 9, 7)},),
            'Prec': (1,)
        },
        {
            'Name': "6",
            'Modes': ({'Duration': 8, 'renRes': (3, 7, 9, 8)},),
            'Prec': (1, 4)
        },
        {
            'Name': "7",
            'Modes': ({'Duration': 1, 'renRes': (10, 4, 8, 3)},),
            'Prec': (3,)
        },
        {
            'Name': "8",
            'Modes': ({'Duration': 7, 'renRes': (10, 10, 3, 1)},),
            'Prec': (3,)
        },
        {
            'Name': "9",
            'Modes': ({'Duration': 4, 'renRes': (4, 1, 4, 5)},),
            'Prec': (2, 7)
        },
        {
            'Name': "10",
            'Modes': ({'Duration': 6, 'renRes': (2, 6, 6, 2)},),
            'Prec': (8,)
        },
        {
            'Name': "11",
            'Modes': ({'Duration': 4, 'renRes': (7, 7, 8, 10)},),
            'Prec': (4, 5, 10)
        },
        {
            'Name': "12",
            'Modes': ({'Duration': 9, 'renRes': (7, 9, 8, 8)},),
            'Prec': (1, 2, 10)
        },
        {
            'Name': "13",
            'Modes': ({'Duration': 10, 'renRes': (8, 1, 3, 9)},),
            'Prec': (0, 5)
        },
        {
            'Name': "14",
            'Modes': ({'Duration': 1, 'renRes': (4, 6, 2, 7)},),
            'Prec': (6, 7, 11)
        },
        {
            'Name': "15",
            'Modes': ({'Duration': 7, 'renRes': (10, 2, 8, 5)},),
            'Prec': (9, 14)
        },
        {
            'Name': "16",
            'Modes': ({'Duration': 8, 'renRes': (6, 3, 3, 8)},),
            'Prec': (6, 8, 13)
        },
        {
            'Name': "17",
            'Modes': ({'Duration': 6, 'renRes': (7, 9, 1, 5)},),
            'Prec': (8, 13)
        },
        {
            'Name': "18",
            'Modes': ({'Duration': 2, 'renRes': (6, 2, 1, 2)},),
            'Prec': (7, 13)
        },
        {
            'Name': "19",
            'Modes': ({'Duration': 5, 'renRes': (6, 5, 9, 1)},),
            'Prec': (0, 5)
        },
        {
            'Name': "20",
            'Modes': ({'Duration': 6, 'renRes': (2, 3, 9, 7)},),
            'Prec': (9, 10, 17, 19)
        },
        {
            'Name': "21",
            'Modes': ({'Duration': 2, 'renRes': (2, 10, 8, 4)},),
            'Prec': (12, 19)
        },
        {
            'Name': "22",
            'Modes': ({'Duration': 2, 'renRes': (10, 10, 7, 7)},),
            'Prec': (2, 16, 20)
        },
        {
            'Name': "23",
            'Modes': ({'Duration': 6, 'renRes': (6, 7, 9, 5)},),
            'Prec': (16, 18, 20)
        },
        {
            'Name': "24",
            'Modes': ({'Duration': 9, 'renRes': (8, 6, 4, 7)},),
            'Prec': (9, 16, 20)
        },
        {
            'Name': "25",
            'Modes': ({'Duration': 4, 'renRes': (7, 5, 7, 7)},),
            'Prec': (12, 17, 19)
        },
        {
            'Name': "26",
            'Modes': ({'Duration': 10, 'renRes': (2, 10, 9, 4)},),
            'Prec': (17, 18, 21)
        },
        {
            'Name': "27",
            'Modes': ({'Duration': 3, 'renRes': (7, 9, 6, 6)},),
            'Prec': (12, 15, 22)
        },
        {
            'Name': "28",
            'Modes': ({'Duration': 5, 'renRes': (10, 4, 6, 4)},),
            'Prec': (4, 25, 26)
        },
        {
            'Name': "29",
            'Modes': ({'Duration': 8, 'renRes': (9, 4, 4, 9)},),
            'Prec': (11, 23, 24)
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

