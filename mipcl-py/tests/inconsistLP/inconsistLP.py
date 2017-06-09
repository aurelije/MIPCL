#!/usr/bin/python
import mipshell
from mipshell import *

class InconsistLP(Problem):
    def model(self):
        x = VarVector([3],'x',(REAL|GE))
        minimize(4*x[0] + x[1] + 2*x[2])

        2*x[0] + 5*x[1] + x[2] <= 5
        x[0] + 2*x[1] >= 3

prob = InconsistLP("inconsistLP")
prob.model()
prob.optimize()
prob.printSolution()

