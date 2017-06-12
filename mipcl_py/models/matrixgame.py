from mipcl_py.mipshell.mipshell import *

class MatrixGame(Problem):
    def LPmodel(self):
        """Formulation of the LP for the input matrix game."""
        A = self.A
        m, n = len(A), len(A[0])
        x = VarVector([n],"x")

        maximize(sum_(x[j] for j in range(n)))

        for i in range(m):
            sum_(A[i][j]*x[j] for j in range(n)) <= 1

    def eqInPureStrats(self):
        """Solving Matrix Games in pure strategies.

        Input parameters:
            - self.A --- Player 1 profit matrix of size mxn.
        Output:
            - self.alpha, self.beta --- the lower and upper game values;
            - self.i0, self.j0 --- if both lists, i0 and j0, are not empty,
              then each pair (i0[i],j0[j]) is an equilibrium. 
        """
        A = self.A
        m, n = len(A), len(A[0])

        r = []
        for i in range(m):
            r.append(min(A[i]))

        c = []
        for j in range(n):
            c.append(max(A[i][j] for i in range(m)))

        self.alpha = alpha = max(r)
        self.beta = beta = min(c)
        if beta - alpha < 1.0e-8: # there exists equilibrium in pure strategies
            flag = True
            self.v = alpha
            self.i0 = [i for i in range(m) if alpha-r[i] < 1.0e-8]
            self.j0 = [j for j in range(n) if c[j]-beta < 1.0e-8]
        else:
            flag = False
        return flag
        
    def eqInMixedStrats(self):
        """Solving Matrix Games in mixed strategies.

        Input parameters:
            - self.A --- Player 1 profit matrix of size mxn;
            - self.alpha --- lower game value.
        Output:
            - self.v --- game value
            - self.p --- list of size m, p[i] is probability of applying strategy i;
            - self.q --- list of size n, q[j] is probability of applying strategy j.
        """
        if self.alpha < 1.0e-3:
            a = -self.alpha+1.0
            A = self.A
            m, n = len(A), len(A[0])
            for i in range(m):
                for j in range(n):
                    A[i][j] += a
        else:
            a = 0.0
        self.LPmodel()
        self.optimize()
        v = 1.0/self.getObjVal()
        self.q = [v*var.val for var in self.vars]
        self.p = [v*ctr.price for ctr in self.ctrs]
        self.v = v - a

    def solveMatrixGame(self,A):
        self.A = A
        self.i0, self.j0 = None, None
        self.p, self.q = None, None
        if not self.eqInPureStrats():
            self.eqInMixedStrats()

    def printSolution(self):
        print('Game value is {:.4f}'.format(self.v))
        if self.i0 is not None: # equilibrium in pure strategies
            print('Optimal pure strategies:')
            print('    of Player 1: ' + repr(self.i0))
            print('    of Player 2: ' + repr(self.j0))
        else: # equilibrium in mixed strategies
            print('Optimal mixed strategies:')
            print('    of Player 1: ' + repr(self.p))
            print('    of Player 2: ' + repr(self.q))


