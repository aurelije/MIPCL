from mipcl_py.mipshell.mipshell import *

class Unitcom(Problem):
    """ Class Unitcom is designed for solving the unit commitment problem.

        Input parameters:
        	d: list of size T, where d[t] is demand in period $t$;
            units: list of size n x T, where, for each unit i,
               l(i) = units[i][0] and u(i) = units[i][1] are minimum and maximum levels
                of per period production;
               r1(i) = units[i][2] and r2(i) = units[i][3] are ramping parameters;
               g(i) = units[i][4] is start-up cost;
               f(i) = units[i][5] and p(i) = units[i][6] are fixed and variable costs.
    """
    def model(self,q,d,units):
        def l(i):
            return units[i][0]
        def u(i):
            return units[i][1]
        def r1(i):
            return units[i][2]
        def r2(i):
            return units[i][3]
        def g(i):
            return units[i][4]
        def f(i):
            return units[i][5]
        def p(i):
            return units[i][6]

        self.units = units
        T, n = len(d), len(units)

        x = VarVector([n,T],"x",BIN)
        self.y = y = VarVector([n,T],"y")
        z = VarVector([n,T],"z",BIN)
	
        minimize(
          sum_(g(i)*z[i][t] + f(i)*x[i][t] + p(i)*y[i][t] for i in range(n) for t in range(T))
        )

        for t in range(T):
            sum_(y[i][t] for i in range(n)) == d[t]
            sum_(u(i)*x[i][t] for i in range(n))  >= q*d[t]
            for i in range(n):
                y[i][t] >= l(i)*x[i][t]
                y[i][t] <= u(i)*x[i][t]
                -r1(i) <= y[i][t] - y[i][(t-1+T) % T] <= r2(i)
                x[i][t] - x[i][(t-1+T) % T] <= z[i][t]
                z[i][t] <= x[i][t]

    def printSolution(self):
        y = self.y
        n, T = len(y), len(y[0])
        print('Optimal objective value: {:.4f}'.format(self.getObjVal()))
        print('\n    Generator powers:')
        print(' ' + '_'*(10*n+9))
        print('|         |' + 'Generators'.center(10*n-1) + '|')
        print('+' + '-'*(10*n+9) + '+')
        str = '| ' + 'Periods'.center(7) + ' |'
        for i in range(n):
            str += repr(i+1).center(8) + ' |'
        print(str)
        print('+' + '---------+'*(n+1))
        for t in range(T):
            str = '| ' + repr(t+1).center(7) + ' |'
            for i in range(n):
                str += format(y[i][t].val,'.2f').rjust(8) + ' |'
            print(str)
        print(' ' + '-'*(10*n+9))

