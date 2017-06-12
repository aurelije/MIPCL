from mipcl_py.mipshell.mipshell import *

class Batch(Problem):
    def model(self,H,tasks,states,units):
        """ Batch Production.

        Input parameters:
          H: integer, length of planning horizon;
           tasks: list of tasks, where task i is characterized as follows:
              J(i)=tasks[i][0]: set of units capable processing task i;
                   tasks[i][1]: dictionary that describes states feeding task i,
                                we define inS(i)=tasks[i][1].items();
                   tasks[i][2]: dictionary that describes states where task i sends its outputs,
                                we define outS(i)=tasks[i][2].items();
             dur(i)=tasks[i][3]: integer, duration of task i	
	  states: list of states, where state s is defined by the following parameters:
              u(s)=states[s][0]: storage capacity;
             z0(s)=states[s][1]: initial stock;
              h(s)=states[s][2]: cost of storing one unit of product;
              c(s)=states[s][3]: price of product.
           units: list of units, where each unit j  is characterized as follows:
               I(j)=units[j][0]: set of tasks that can be performed by the unit;
                 units[j][1][0] and units[j][1][0]: minimum and maximum loads for this unit.
        """
        def u(s):
            return states[s][0]
        def z0(s):
            return states[s][1]
        def h(s):
            return states[s][2]
        def c(s):
            return states[s][3]
        def J(i):
            return tasks[i][0]
        def inS(i):
            return tasks[i][1].items()
        def outS(i):
            return tasks[i][2].items()
        def dur(i):
            return tasks[i][3]
        def I(j):
            return units[j][0]

        self.H = H
        self.tasks, self.states, self.units = tasks, states, units
        STATES, TASKS, UNITS = states.keys(), tasks.keys(), units.keys()

        self.z = z = {}
        for s in STATES:
            for t in range(H+1):
                z[s,t] = Var('z' + str((s,t)))
        self.x = x = {}
        self.y = y = {}
        for i in TASKS:
            for j in J(i):
                for t in range(H-dur(i)+1):
                    x[i,j,t] = Var('x' + str((i,j,t)),BIN)
                    y[i,j,t] = Var('y' + str((i,j,t)))
        
        maximize(
            sum_(c(s)*z[s,H] for s in STATES) 
	    - sum_(h(s)*z[s,t] for s in STATES for t in range(H))
        )

        for t in range(H):
            for j in UNITS:
                sum_(x[i,j,t] for i in I(j) if t+tasks[i][3] <= H) <= 1

        for j in UNITS:
            Vmin, Vmax = units[j][1][0], units[j][1][1]
            for i in I(j):
                for t in range(H-dur(i)+1):
                    y[i,j,t] >= Vmin*x[i,j,t]
                    y[i,j,t] <= Vmax*x[i,j,t]
            for t in range(H):
                sum_(x[i,j,tau] for i in I(j)\
                    for tau in range(max(0,t-dur(i)+1),min(t,H-dur(i)))) <= 1

        for s in STATES:
            for t in range(H): 
                z[s,t] <= u(s)
            z[s,0] + sum_(rho*y[i,j,0] for i in TASKS for j in J(i)\
                 for s1,rho in inS(i) if s1 == s) == z0(s)
            for t in range(1,H+1):
                z[s,t-1] + sum_(rho*y[i,j,t-d] for i in TASKS for j in J(i)\
                     for s1,(rho,d) in outS(i) if s1 == s and t >= d) ==\
		z[s,t] + sum_(rho*y[i,j,t] for i in TASKS for j in J(i)\
                     for s1,rho in inS(i) if s1 == s and t+dur(i) <= H)

    def printSolution(self):
        def dur(i):
            return tasks[i][3]
        def I(j):
            return units[j][0]

        H = self.H
        states, tasks, units = self.states, self.tasks, self.units
        z, y = self.z, self.y
        print('Objective value = {0:.4f}'.format(self.getObjVal()))
        print('\nProducts produced:')
        for s in states.keys():
            print('{0:.4f} in state {1}'.format(z[s,H].val,repr(s)))

        print('\nSchedule:')
        for j in units.keys():
            print('\n' + repr(j) + ':')
            for t in range(H):
                for i in I(j):
                    if t+dur(i) <= H:
                        if y[i,j,t].val > 1.0e-6:
                            print('batch of task {0} starts at {1}, size = {2:.4f}'.format(repr(i),repr(t),y[i,j,t].val))

