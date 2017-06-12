from mipcl_py.mipshell.mipshell import *

class BimatrixGame(Problem):
    def model(self, A):
        """A MIP formulation for the bimatrix game.

        The goal is to find an equilibrium for which
        the minimum of player's gains is maximum.

        Args:
           A (2-dimensional list of pairs of float): matrix of players gains:
           in situation ``(i,j)`` Player 1 wins ``A[i][j][0]``,
           and Player 2 wins ``A[i][j][1]``.  

        """
        self.A = A
        m, n = len(A), len(A[0])
        self.p = p = VarVector([m],"p")
        self.q = q = VarVector([n],"q")
        x = VarVector([m],"x",BIN)
        y = VarVector([n],"y",BIN)
        self.v1 = v1 = Var("v1",lb=-VAR_INF)
        self.v2 = v2 = Var("v2",lb=-VAR_INF)
        u = Var("u",lb=-VAR_INF)

        U1 = max(A[i][j][0] for i in range(m) for j in range(n)) -\
             min(A[i][j][0] for i in range(m) for j in range(n))
        U2 = max(A[i][j][1] for i in range(m) for j in range(n)) -\
             min(A[i][j][1] for i in range(m) for j in range(n))

        maximize(u)

        for i in range(m):
            p[i] + x[i] <= 1
            sum_(A[i][j][0]*q[j] for j in range(n)) <= v1
            sum_(A[i][j][0]*q[j] for j in range(n)) >= v1 - U1*x[i]

        for j in range(n):
            q[j] + y[j] <= 1
            sum_(A[i][j][1]*p[i] for i in range(m)) <= v2
            sum_(A[i][j][1]*p[i] for i in range(m)) >= v2 - U2*y[j]

        sum_(p[i] for i in range(m)) == 1
        sum_(q[j] for j in range(n)) == 1

        v1 >= u
        v2 >= u

    def printSolution(self):
        print('Player 1 gain: {:.4f}'.format(self.v1.val))
        print('Player 2 gain: {:.4f}'.format(self.v2.val))
        print('Optimal mixed strategies:')
        p, q = self.p, self.q
        print('    of Player 1: ' + repr(['{:.4f}'.format(var.val) for var in p]))
        print('    of Player 2: ' + repr(['{:.4f}'.format(var.val) for var in q]))


