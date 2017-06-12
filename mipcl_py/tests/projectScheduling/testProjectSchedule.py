#!/usr/bin/python
from mipcl_py.models.projectScheduling import projectSchedule

project = {
  'Name': 'project0',
  'Horizon': 36,
    'renRes': (
        {'Name': "1", 'Quantity': 6},
        {'Name': "2", 'Quantity': 7},
        {'Name': "3", 'Quantity': 6}
    ),
  'nonRenRes': ({'Nane': "1", 'Quantity': 3},),
  'Jobs': (
     {
       'Name': "0",
       'Modes': (
           {'Duration': 3, 'renRes': (3, 2, 1), 'nonRenRes': (0,)},
           {'Duration': 2, 'renRes': (3, 2, 4), 'nonRenRes': (1,)}
       ),
       'Prec': ()
     },
     {
       'Name': "1",
       'Modes': ({'Duration': 5, 'renRes': (2, 4, 2), 'nonRenRes': (0,)},),
       'Prec': ()
     },
     {
       'Name': "2",
       'Modes': ({'Duration': 6, 'renRes': (3, 1, 2), 'nonRenRes': (0,)},),
       'Prec': (0,)
     },
     {
       'Name': "3",
       'Modes': ({'Duration': 2, 'renRes': (4, 3, 1), 'nonRenRes': (0,)},),
       'Prec': (0,)
     },
     {
       'Name': "4",
       'Modes': ({'Duration': 3, 'renRes': (2, 0, 3), 'nonRenRes': (0,)},),
       'Prec': (3,)
     },
     {
       'Name': "5",
       'Modes': ({'Duration': 3, 'renRes': (1, 1, 1), 'nonRenRes': (0,)},),
       'Prec': (3,)
     },
     {
       'Name': "6",
       'Modes': (
           {'Duration': 4, 'renRes': (3, 1, 1), 'nonRenRes': (0,)},
           {'Duration': 3, 'renRes': (3, 2, 1), 'nonRenRes': (1,)}
       ),
       'Prec': (1,)
     },
     {
       'Name': "7",
       'Modes': (
           {'Duration': 4, 'renRes': (3, 2, 3), 'nonRenRes': (0,)},
           {'Duration': 2, 'renRes': (3, 3, 3), 'nonRenRes': (1,)}
       ),
       'Prec': (2,)
     },
     {
       'Name': "8",
       'Modes': ({'Duration': 2, 'renRes': (4, 1, 0), 'nonRenRes': (0,)},),
       'Prec': (4,7)
     },
     {
       'Name': "9",
       'Modes': (
           {'Duration': 5, 'renRes': (2, 2, 2), 'nonRenRes': (0,)},
           {'Duration': 3, 'renRes': (3, 2, 3), 'nonRenRes': (1,)}
       ),
       'Prec': (5,6)
     },
     {
       'Name': "10",
       'Modes': ({'Duration': 3, 'renRes': (5, 4, 2), 'nonRenRes': (0,)},),
       'Prec': (8,)
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

