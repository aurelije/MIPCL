from mipcl_py.mipshell.mipshell import *

class GenAssign(Problem):
    def model(self,l,p,c):
        """ Generalised assignmen problem.

            Input:
                l[j]: workload of machine j,
                p[i][j]: processing time of job i on machine j,
                c[i][j]: cost of processing job i on machine j.
        """
        m, n = len(c), len(c[0])
        self.x = x = VarVector([m,n],"x",BIN)

        minimize(sum_(c[j][i]*x[j][i] for i in range(n) for j in range(m)))

        for j in range(m):
            sum_(x[j][i] for i in range(n)) == 1

        for i in range(n):
            sum_(p[j][i]*x[j][i] for j in range(m)) <= l[i]

    def printSolution(self):
        x = self.x
        m, n = len(x), len(x[0])
        print('Objective value is {0:.2f}'.format(self.getObjVal()))
        print('Shedule:')
        for i in range(n):
            str = '======== Machine {:d} processes jobs:'.format(i+1)
            for j in range(m):
                if x[j][i].val > 0.5:
                    str += ' {:d}'.format(j+1)
            print(str)


