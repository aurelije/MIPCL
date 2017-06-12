from mipcl_py.mipshell.mipshell import *

class MultLotSize(Problem):
    """Multiproduct Lot-Sizing.

    For product j we know:
        product[j][0][0]: initial stock
        product[j][0][1]: final stock
        product[j][0][2]: storage capacity
        products[j][1][t]: demand in period t
        products[j][2][t]: unit production cost in period t
        products[j][3][t]: unit holding cost in period t
        products[j][4][k]: quantity of product j used in production of one unit of product k
    For machines of type i we know:
        machines[i][0][t]: number of machines available in period t
        machines[i][1][t]: cost of running one machine in period t
        machines[i][2][j]: time needed to produce one unit of product j
        
    It is also given:
        l[t]: duration of period t
    """
    def model(self,l,products,machines):
        def s0(j):
            return products[j][0][0]
        def sf(j):
            return products[j][0][1]
        def u(j):
            return products[j][0][2]
        def d(j,t):
            return products[j][1][t]
        def c(j,t):
            return products[j][2][t]
        def h(j,t):
            return products[j][3][t]
        def rho(j,k):
            return products[j][4][k]
        def mn(i,t):
            return machines[i][0][t]
        def f(i,t):
            return machines[i][1][t]
        def tau(i,j):
            return machines[i][2][j]

        self.products, self.machines = products, machines
        m, n, T = len(machines), len(products), len(l)
        self.s = s = VarVector([n,T],"s")
        self.x = x = VarVector([n,T],"x",INT)
        self.y = y = VarVector([m,T],"y",INT)

        minimize(
            sum_(c(j,t)*x[j][t] + h(j,t)*s[j][t] for j in range(n) for t in range(T)) + \
	    sum_(f(i,t)*y[i][t] for i in range(m) for t in range(T)) 
	)

        for j in range(n):
            s0(j) + x[j][0] == d(j,0) + s[j][0]\
            + sum_(rho(j,k)*x[k][0] for k in range(n) if rho(j,k) > ZERO) 
		
        for t in range(1,T):
            for j in range(n):
                s[j][t-1] + x[j][t] == d(j,t) + s[j][t]\
                + sum_(rho(j,k)*x[k][t] for k in range(n) if rho(j,k) > ZERO) 

        for i in range(m):
            for t in range(T):
                sum_(tau(i,j)*x[j][t] for j in range(n) if tau(i,j) > ZERO) <= l[t]*y[i][t]
                y[i][t] <= mn(i,t)
		
        for j in range(n):
            for t in range(T-1):
                s[j][t] <= u(j)
            s[j][T-1] == sf(j)

    def printTbl(self, tblName, rowTitle, colTitle, x):
        n, T = len(x), len(x[0])
        print('\n    ' + tblName + ':')
        print(' ' + '_'*(10*n+8))
        print('|        |' + rowTitle.center(10*n-1) + '|')
        print('|' + '-'*(10*n+8) + '|')
        str = '| ' + colTitle.center(6) + ' |'
        for j in range(n):
            str += repr(j+1).center(8) + ' |'
        print(str)
        print('|--------+' + '---------+'*(n-1) + '---------|')
        for t in range(T):
            str = '| ' + repr(t+1).center(6) + ' |'
            for j in range(n):
                str += format(x[j][t].val,'.2f').rjust(8) + ' |'
            print(str)
        print(' ' + '-'*(10*n+8))
  
    def printSolution(self):
        print('Objective value = {:.4f}'.format(self.getObjVal()))
        self.printTbl('Production','Products','Period',self.x)
        self.printTbl('Stock','Products','Period',self.s)
        self.printTbl('Machines','Machines','Period',self.y)



