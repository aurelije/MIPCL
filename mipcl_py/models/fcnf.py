from mipcl_py.mipshell.mipshell import *

def t(e):
    return e[0]
def h(e):
    return e[1]
def u(e):
    return e[2]
def f(e):
    return e[3]
def c(e):
    return e[4]

class Fcnf(Problem):
    """The class for solving fixed charge network flow problems.
    """
    def model(self,G):
        """Build a model for the fixed charge network flow problem.

        Args:
           G (:obj:`dictionary`): flow network.

           G['Demands'] (:obj:`list` of :obj:`int`): list of demands at nodes.
           G['Arcs'] (:obj:`list` of :obj:`tuple`): list of edges:
              G['Arcs'][j] (:obj:`tuple` of :obj:`int`): tuple of five integers.
                 G['Arcs'][j][0] (int): tail of edge j.
                 G['Arcs'][j][1] (int): head of edge j.
                 G['Arcs'][j][2] (int): capacity of edge j.
                 G['Arcs'][j][3] (int): cost of edge j.
                 G['Arcs'][j][4] (int): fixed cost of edge j.

        Returnes:
           None.
        """
        self.G = G
        d, E = G['Demands'], G['Arcs']
        m, n = len(E), len(d)

        self.x = x = VarVector([m],"x")
        y = VarVector([m],"y",BIN)

        minimize(sum_(f(e)*y[j] + c(e)*x[j] for j,e in enumerate(E)))

        for v in range(n):
            sum_(x[j] for j,e in enumerate(E) if  h(e)==v)\
          - sum_(x[j] for j,e in enumerate(E) if  t(e)==v) == d[v]
		
        for j,e in enumerate(E):
            x[j] <= u(e)*y[j]

    def printSolution(self):
        """Prints the solution.
        """
        if self.is_solution is not None:
            if self.is_solution == True:
                d, E = self.G['Demands'], self.G['Arcs']
                x = self.x
                print('Flow cost = {:d}'.format(int(self.getObjVal() + 0.5)))
                for j,e in enumerate(E):
                    if x[j].val > 0.5:
                        print('flow({:d},{:d}) = {:d}'.format(t(e),h(e),int(x[j].val + 0.5)))
            else:
                print('Problem has no solution!')
        else:
            print('Please run optimize first')


