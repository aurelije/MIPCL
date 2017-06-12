from mipcl_py.mipshell.mipshell import *

class Proclayout(Problem):
    def model(self,p,dist,flows):
        """ Process Layout Problem.

            Input parameters:
                p: list of size n x m,
                   where p[i][s] is cost of moving department i to site s;
	     dist: list that represents an upper triangular square matrix of size m,
	           where dist[s][r-(s+1)] is distance between sites s and r, s < r;
	    flows: list that represents an upper triangular square matrix of size $n$,
	           where flows[i][j-(i+1)] is the volume of material moving between departments i and j, i < j.
        """
        def c(i,j,s,r):
            if s == r:
                cost = 0
            else:
                if s > r:
                    k = s
                    s = r
                    r = k
                cost = dist[s][r-(s+1)] * flows[i][j-(i+1)]
            return cost

        self.dist, self.flows = dist, flows
        n, m = len(p), len(p[0])
        self.x = x = VarVector([n,m],"x",BIN)
        y = VarVector([n,n,m,m],"y",BIN)

        minimize(
            sum_(p[i][s]*x[i][s] for i in range(n) for s in range(m)) +\
            sum_(c(i,j,s,r)*y[i][j][s][r] for i in range(n-1) for j in range(i+1,n) \
                                          for s in range(0,m) for r in range(0,m))
	)
	 
        for i in range(n):
            sum_(x[i][s] for s in range(m)) == 1
		
        for s in range(m):
            sum_(x[i][s] for i in range(n)) <= 1
		
        for i in range(n-1):
            for j in range(i+1,n):
                sum_(y[i][j][s][r] for s in range(m) for r in range(m)) == 1 

        for i in range(n-1):
            for j in range(i+1,n):
                 for s in range(m):
                     for r in range(m):
                         2*y[i][j][s][r] - x[i][s] - x[j][r] <= 0

    def printSolution(self):
        x = self.x
        n, m = len(x), len(x[0])

        print('Total expenses: {0:.4f}'.format(self.getObjVal()))
        print(' ___________________')
        print('| Depart. |  Site   |')
        print('|---------+---------|')
        for i in range(n):
            for s in range(m):
               if x[i][s].val > 0.5:
                   print('| {0:7d} | {1:7d} |'.format(i+1,s+1))
        print(' -------------------')


