#!/usr/bin/python
from mipcl_py.models.projectScheduling import projectSchedule

project = {
    'Name': 'project1',
    'Horizon': 100,
    'renRes': (
        {'Name': "1", 'Quantity': 12},
        {'Name': "2", 'Quantity': 13},
        {'Name': "3", 'Quantity': 4},
        {'Name': "4", 'Quantity': 12}
    ),
    'nonRenRes': ({'Name': "1", 'Quantity': 5},),
    'Jobs': (
        {
            'Name': "0",
            'Modes': (
                {'Duration': 8, 'renRes': (4, 0, 0, 7), 'nonRenRes': (0,)},
                {'Duration': 5, 'renRes': (4, 1, 2, 6), 'nonRenRes': (1,)}
            ),
            'Prec': ()
        },
        {
            'Name': "1",
            'Modes': ({'Duration': 4, 'renRes': (10, 0, 3, 5), 'nonRenRes': (0,)},),
            'Prec': ()
        },
        {
            'Name': "2",
            'Modes': ({'Duration': 6, 'renRes': (0, 5, 0, 3), 'nonRenRes': (0,)},),
  			'Prec': ()
        },
        {
            'Name': "3",
            'Modes': ({'Duration': 3, 'renRes': (3, 0, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (2,)
        },
        {
            'Name': "4",
            'Modes': (
                {'Duration': 8, 'renRes': (0, 6, 2, 8), 'nonRenRes': (0,)},
                {'Duration': 6, 'renRes': (2, 6, 4, 8), 'nonRenRes': (1,)}
            ),
            'Prec': (0,)
        },
        {
            'Name': "5",
            'Modes': ({'Duration': 5, 'renRes': (4, 0, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (1,)
        },
        {
            'Name': "6",
            'Modes': (
                {'Duration': 9, 'renRes': (0, 4, 1, 2), 'nonRenRes': (0,)},
                {'Duration': 6, 'renRes': (1, 4, 2, 2), 'nonRenRes': (1,)}
        ),
            'Prec': (5,)
        },
        {
            'Name': "7",
            'Modes': ({'Duration': 2, 'renRes': (6, 0, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (2,)
        },
        {
            'Name': "8",
            'Modes': ({'Duration': 7, 'renRes': (0, 0, 3, 1), 'nonRenRes': (0,)},),
            'Prec': (2,)
        },
        {
            'Name': "9",
            'Modes': ({'Duration': 9, 'renRes': (0, 5, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (0,)
        },
        {
            'Name': "10",
            'Modes': ({'Duration': 2, 'renRes': (0, 7, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (6,)
        },
        {
            'Name': "11",
            'Modes': ({'Duration': 6, 'renRes': (4, 0, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (1, 2)
        },
        {
            'Name': "12",
            'Modes': ({'Duration': 3, 'renRes': (0, 8, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (7, 10)
        },
        {
            'Name': "13",
            'Modes': (
                {'Duration': 9, 'renRes': (3, 2, 0, 0), 'nonRenRes': (0,)},
                {'Duration': 7, 'renRes': (3, 2, 1, 1), 'nonRenRes': (1,)}
            ),
            'Prec': (0,)
        },
        {
            'Name': "14",
            'Modes': (
                {'Duration': 10, 'renRes': (0, 4, 0, 5), 'nonRenRes': (0,)},
                {'Duration': 8, 'renRes': (1, 4, 1, 5), 'nonRenRes': (1,)}
            ),
            'Prec': (8,)
        },
        {
            'Name': "15",
            'Modes': ({'Duration': 6, 'renRes': (0, 0, 0, 8), 'nonRenRes': (0,)},),
            'Prec': (11, 12)
        },
        {
            'Name': "16",
            'Modes': ({'Duration': 5, 'renRes': (0, 3, 0, 7), 'nonRenRes': (0,)},),
            'Prec': (15,)
        },
        {
            'Name': "17",
            'Modes': ({'Duration': 3, 'renRes': (0, 1, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (6,)
        },
        {
            'Name': "18",
            'Modes': (
                {'Duration': 7, 'renRes': (0, 10, 0, 0), 'nonRenRes': (0,)},
                {'Duration': 5, 'renRes': (0, 9, 1, 2), 'nonRenRes': (1,)}
            ),
            'Prec': (3, 9, 16)
        },
        {
            'Name': "19",
            'Modes': ({'Duration': 2, 'renRes': (0, 6, 0, 2), 'nonRenRes': (0,)},),
            'Prec': (14,)
        },
        {
            'Name': "20",
            'Modes': ({'Duration': 7, 'renRes': (2, 0, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (14, 15, 16)
        },
        {
            'Name': "21",
            'Modes': ({'Duration': 2, 'renRes': (3, 0, 4, 0), 'nonRenRes': (0,)},),
            'Prec': (18, 20)
        },
        {
            'Name': "22",
            'Modes': ({'Duration': 3, 'renRes': (0, 9, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (17, 21)
        },
        {
            'Name': "23",
            'Modes': ({'Duration': 3, 'renRes': (4, 0, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (8, 13, 18)
        },
        {
            'Name': "24",
            'Modes': (
                {'Duration': 7, 'renRes': (0, 0, 4, 0), 'nonRenRes': (0,)},
                {'Duration': 5, 'renRes': (0, 1, 4, 2), 'nonRenRes': (1,)},
            ),
            'Prec': (9,)
        },
        {
            'Name': "25",
            'Modes': (
                {'Duration': 8, 'renRes': (0, 0, 0, 7), 'nonRenRes': (0,)},
                {'Duration': 6, 'renRes': (1, 0, 2, 7), 'nonRenRes': (1,)}
            ),
            'Prec': (5, 6)
        },
        {
            'Name': "26",
            'Modes': ({'Duration': 3, 'renRes': (0, 8, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (19, 25)
        },
        {
            'Name': "27",
            'Modes': ({'Duration': 7, 'renRes': (0, 7, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (17,)
        },
        {
            'Name': "28",
            'Modes': ({'Duration': 2, 'renRes': (0, 7, 0, 0), 'nonRenRes': (0,)},),
            'Prec': (4, 22, 23)
        },
        {
            'Name': "29",
            'Modes': ({'Duration': 2, 'renRes': (0, 0, 2, 0), 'nonRenRes': (0,)},),
            'Prec': (24, 26)
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

