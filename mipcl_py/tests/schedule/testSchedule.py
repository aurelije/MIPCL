#!/usr/bin/python
from mipcl_py.models.schedule import Schedule

jobs = {
  0: [0,  4, 2, {0: 2, 1: 3}, ()],
  1: [0,  6, 1, {0: 1, 1: 4}, ()],
  2: [0,  5, 3, {0: 2, 1: 2}, ()],
  3: [3,  7, 1, {0: 2, 1: 1}, ()],
  4: [2,  8, 3, {0: 4, 1: 3}, ()],
  5: [6, 12, 2, {0: 3, 1: 4}, ()],
  6: [1, 10, 4, {0: 5, 1: 4}, ()],
  7: [2,  8, 2, {0: 4, 1: 2}, ()],
  8: [0,  9, 1, {0: 1, 1: 3}, ()],
  9: [5, 12, 3, {0: 5, 1: 5}, ()]
}

prob = Schedule("test1")
prob.model(jobs)
prob.optimize()
prob.printSolution()

