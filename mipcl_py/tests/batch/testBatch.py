#!/usr/bin/python
from mipcl_py.models.batch import Batch

H = 9 # time horizon

tasks = {
    'Heating': (
        {'Heater'},
        {'Fead_A': 1.0},
        {'Hot_A': (1.0, 1)},
        1
    ),
    'Reaction_1': (
        {'Reactor_1', 'Reactor_2'},
        {'Fead_B': 0.5, 'Fead_C': 0.5},
        {'Int_BC': (1.0, 2)},
        2
    ),
    'Reaction_2': (
        {'Reactor_1', 'Reactor_2'},
        {'Hot_A': 0.4, 'Int_BC': 0.6},
        {'Int_AB': (0.6, 2), 'Prod_1': (0.4, 2)},
	2
    ),
    'Reaction_3': (
        {'Reactor_1', 'Reactor_2'},
        {'Fead_C': 0.2, 'Int_AB': 0.8},
        {'Impure_E': (1.0, 1)},
        1
    ),
    'Separation': (
        {'Still'},
        {'Impure_E': 1.0},
        {'Int_AB': (0.1, 2), 'Prod_2': (0.9, 1)},
        1
    )
}

# Numbers in the four-tuples are:
#   0) storage capacity,
#   1) initial stock,
#   2) cost of storing a unit amount of material,
#   3) price of product produced in the state.
states = {
      'Fead_A': (1000, 1000, 0.0,  0.0),
      'Fead_B': (1000, 1000, 0.0,  0.0),
      'Fead_C': (1000, 1000, 0.0,  0.0),
       'Hot_A': ( 100,  0.0, 0.1, -1.0),
      'Int_AB': ( 200,  0.0, 0.1, -1.0),
      'Int_BC': ( 150,  0.0, 0.1, -1.0),
    'Impure_E': ( 100,  0.0, 0.1, -1.0),
      'Prod_1': (1000,  0.0, 0.0, 10.0),
      'Prod_2': (1000,  0.0, 0.0, 10.0)
}

# Each unit is described by:
#   1) the set of tasks that this unit is capable to fulfill,
#   2) the 2-tuple which numbers are minimum and maximum loads for this unit.    
units = {
       'Heater': ({'Heating'}, (0, 100)),
    'Reactor_1': ({'Reaction_1', 'Reaction_2', 'Reaction_3'}, (0, 80)),
    'Reactor_2': ({'Reaction_1', 'Reaction_2', 'Reaction_3'}, (0, 50)),
        'Still': ({'Separation'}, (0, 200))
}

prob = Batch("STN1")
prob.model(H,tasks,states,units)
prob.optimize()
prob.printSolution()

