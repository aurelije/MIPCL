from mipcl_py.mipshell.mipshell import *

class Transport(Problem):
    def model(self,a,b,c):
        m, n = len(a), len(b)
        self.x = x = VarVector([m,n],"x",INT)

        minimize(sum_(c[i][j]*x[i][j] for j in range(0,n) for i in range(0,m)))

        for i in range(0,m):
            sum_(x[i][j] for j in range(0,n)) <= a[i] 

        for j in range(0,n):
            sum_(x[i][j] for i in range(0,m)) == b[j]

    def printSolution(self):
        if self.is_solution != None:
            if self.is_solution == True:
                print('Total supplying cost  = ' + repr(self.getObjVal()))
                print('-'*33)
                x = self.x
                m, n = len(x), len(x[0])
                for i in range(0,m):
                    print('Supplier ' + repr(i) + ' sends')
                    for j in range(0,n):
                        if x[i][j].val > 0.0:
                            print(' ' + repr(x[i][j].val).rjust(10) + ' units to customer ' + repr(j))
                    print('-'*33)
            else:
                print('This problem has no feasible solution')
        else:
            print('Please run optimize first')


