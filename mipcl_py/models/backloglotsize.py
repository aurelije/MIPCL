from mipcl_py.mipshell.mipshell import *

class Backloglotsize(Problem):
    def model(self,d,u,f,c):
        """Lot-Sizing with Backordering.

        Input parameters:
            d: list of size T, where d[t] is demand for product in period t;
            u: list of size T, where u[t] is production capacity in period t;
            f: list of size T, where f[t] is fixed cost of starting production in period t;
            c: list of size TxT, where c[tau][t] is cost of producing one unit of product in period t,
               and then supplying that unit to meet demand for period tau.
        """
        self.f, self.c = f, c
        T, Tp = len(d), len(u)
        self.x = x = VarVector((Tp,T),"x")
        self.y = y = VarVector([Tp],"y",BIN)

        minimize(
            sum_(c[tau][t]*x[tau][t] for tau in range(Tp) for t in range(T) if c[tau][t] >= 0) +\
	    sum_(f[tau]*y[tau] for tau in range(Tp))
        )
	
        for t in range(T):
            sum_(x[tau][t] for tau in range(Tp)) == d[t]
		
        for tau in range(Tp):
            sum_(x[tau][t] for t in range(T)) <= u[tau]*y[tau]

        for tau in range(Tp):
            for t in range(T):
                if c[tau][t] < 0:
                     x[tau][t] == 0;

    def printSolution(self):
        f, c = self.f, self.c
        x, y = self.x, self.y
        Tp, T = len(x), len(x[0])

        prodCost, fxCost = 0, 0
        print(' ________________________________')
        print('| Prod.  | produces | for demand |')
        print('| period | (units)  |   period   |')
	
        for tau in range(Tp):
            prod=k=0;
            if y[tau].val > 0.5:
                print('|--------+----------+------------|')
                fxCost += f[tau]
                for t in range(T):
                    val = int(x[tau][t].val)
                    if val > 0:
                        prod += val
                        prodCost += c[tau][t]*val
                        if k > 0:
                            str = '|        | '
                        else:
                            str = '|  {:4d}  | '.format(tau)
                        print(str + '{:8d} |    {:4d}    |'.format(val,t+1))
                        k += 1
        print(' -------------------------------- ')
        print('Production cost - ' + repr(prodCost))
        print('Fixed cost      - ' + repr(fxCost))

