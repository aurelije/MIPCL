import mipshell
from mipshell import *

class Assembly(Problem):
    def model(self,m,C,operations):
        """ Simple Assembly Line Balancing Problem.

        Input parameters:
	    m: integer, maximum number of stations;
	    C: integer, cycle time;
	    operations: dictionary of operations, which item operations[o] is a pair, (t,prec),
                      that describes operation o as follows:
                t: processing time (integer);
                prec: tuple (or list) of preceding operations.
        """
        def t(o):
            return operations[o][0]
        def prec(o):
            return operations[o][1]

        n = len(operations)
        self.x = x = VarVector([m,n],"x",BIN)
        y, z = VarVector([m],"y",BIN), VarVector([n],"z")

        minimize(sum_(y[s] for s in range(m)))

        for o in range(n):
            sum_(x[s][o] for s in range(m)) == 1
            sum_((s+1)*x[s][o] for s in range(m)) == z[o]
            for o1 in prec(o):
                z[o1] <= z[o]


        for s in range(m):
            sum_(t(o)*x[s][o] for o in range(n)) <= C*y[s]
            for o in range(n):
                x[s][o] <= y[s]
	
        for s in range(1,m):
            y[s-1] >= y[s]

    def printSolution(self):
        x = self.x
        n = len(x[0])

        k = int(self.getObjVal()+0.5)
        print('Number of stations = {:d}'.format(k))

        print('Station: assigned operations')
        for s in range(k):
            str = repr(s) + ':'
            for o in range(n):
                if x[s][o].val > 0.5:
                    str += ' ' + repr(o)
            print(str)


