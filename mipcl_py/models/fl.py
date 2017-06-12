from mipcl_py.mipshell.mipshell import *

class Fl(Problem):
    def model(self,q,b,u,f,c):
        """ Service Facility Location.

        Input parameters:
            q: integer, maximum number of depots;
            b: list of size n, where b[j] is number of customers at location j;
            u: list of size m, where u[i] is depot capacity at site i;
            f: list of size m, where f[i] is fixed cost of locating depot at site i;
            c: list of size mxn, where c[i][j] is cost of serving from site i 
	       one customer from location j.
        """
        n, m = len(b), len(u)
        self.y = y = VarVector([m],'y',BIN)
        self.x = x = VarVector([m,n],'x',INT)
	
        minimize(
            sum_(f[i]*y[i] for i in range(m)) + \
            sum_(c[i][j]*x[i][j] for i in range(m) for j in range(n))
        )
    
        sum_(y[i] for i in range(m)) <= q
	
        for j in range(n):
            sum_(x[i][j] for i in range(m)) == b[j]

        for i in range(m):
            sum_(x[i][j] for j in range(n)) <= u[i]*y[i]

        for i in range(m):
            for j in range(n):
                x[i][j] <= min(u[i],b[j])*y[i]


    def printSolution(self):
        y, x = self.y, self.x
        m, n = len(y), len(x[0])

        print('Objective value: {0:.2f}'.format(self.getObjVal()))
        for i in range(m):
            if y[i].val > 0.5:
                print('\n===== Facility at site {!r} serves customers:'.format(i+1))
                for j in range(n):
                    if x[i][j].val > 0.5:
                        print('\t{:.0f} from loc. {!r}'.format(x[i][j].val,j+1))


