from mipcl_py.mipshell.mipshell import *

class Revenue(Problem):
    """Revenue (Yield) Management."""
    def __init__(self,name,planes,options,nodes):
        """ The constructor.

        Args:
            name (str): problem name;
            planes (tuple): list of plane types, where planes[k] is structured as follows:
                q(k,i)=planes[k]['classes'][i][0]: number of places in class i; 
                r(k,i)=planes[k]['classes'][i][1]: number of seats in class i
                     that can be transformed into seats of adjacent classes;
                f(k)=planes[k]['fixedCost']: cost to hire one plane;	  
                     options (tuple): list of options;
            options (tuple): list of size T, where options[t] is list (or tuple) of 3 options, where
                options[t][o]: 3-tuple that represents prices tickets for each of 3 classes;
            nodes (tuple): list of nodes of a scenario tree, each node j, except for node 0,
	      is characterized by a list (or tuple) nodes[j] which is structured as follows: 
                nodes[j]['name']: name;
                nodes[j]['period']: period;
                nodes[j]['parent']: index of the parent node;
                nodes[j]['prob']: probability of reaching this node from its parent;
                nodes[j]['demands'][0]: 3-tuple, demands for tickets for each of three classes
                         if Option~1 is applied;
                nodes[j]['demands'][1]: 3-tuple, demands for tickets for each of three classes
                         if Option~2 is applied;
      	        nodes[j]['demands'][2]: 3-tuple, demands for tickets for each of three classes
                         if Option~3 is applied.
        """
        Problem.__init__(self,name)
        self.planes, self.options, self.nodes = planes, options, nodes
        self.processData()

    def processData(self):
        """ This function does some computations needed for posing the model.

        The function computes probabilities of reaching all non-root scenario-tree nodes.
          The probability of reaching node j is
          the probability of reaching its parent node parent[j]
          multiplied by the probability of moving along the edge (j,parent[j]).

        The function also determines the number, n0, of internal nodes (not leaves).
        """
        nodes = self.nodes
        n0, n, T = -1, len(nodes), len(self.options),
        for j in range(1,n):
            nodes[j]['prob'] *= nodes[nodes[j]['parent']]['prob']
            if n0 < 0:
                if nodes[j]['period'] == T:
                    n0 = j
        self.n0 = n0 # number of internal nodes (not leaves)

    def model(self):
        def c(j,o,i): return nodes[j]['prob']*options[nodes[j]['period']-1][o][i]
        def d(j,i,o): return nodes[j]['demands'][o][i]
        def parent(j): return nodes[j]['parent']
        def q(k,i): return planes[k]['classes'][i]['capacity'][0]
        def tl(k,i): return planes[k]['classes'][i]['capacity'][1]
        def th(k,i): return planes[k]['classes'][i]['capacity'][2]
        def f(k): return planes[k]['fixedCost']

        planes, options, nodes = self.planes, self.options, self.nodes
        K, T, O, I = len(planes), len(options), len(options[0]), len(options[0][0])
        n, n0 = len(nodes), self.n0

        self.v = v = VarVector([K],"v",BIN)
        u = VarVector([I],"u",INT)
        wl, wh = VarVector([I],"wl",INT), VarVector([I],"wh",INT)
        self.x = x = VarVector([n,O,I],"x",INT) # x[0] is not used
        self.y = y = VarVector([n0,O],"y",BIN)
        z = VarVector([n,I],"z",INT)

        maximize(
	   sum_(c(j,o,i)*x[j][o][i] for j in range(1,n) for o in range(O) for i in range(I))\
	   - sum_(f(k)*v[k] for k in range(K))\
        )
	
        sum_(v[k] for k in range(K)) == 1
        for i in range(I):
            u[i] == sum_(q(k,i)*v[k] for k in range(K))
            wl[i] <= sum_(tl(k,i)*v[k] for k in range(K))
            wh[i] <= sum_(th(k,i)*v[k] for k in range(K))

        for j in range(n0):
            sum_(y[j][o] for o in range(O)) == 1

        for j in range(1,n):
            for o in range(O):
                for i in range(I):
                    x[j][o][i] <= d(j,o,i)*y[parent(j)][o]

        for i in range(I): z[0][i] == 0

        for j in range(1,n):
            for i in range(I):
                z[j][i] == z[parent(j)][i] + sum_(x[j][o][i] for o in range(O))
	
        for j in range(n0,n):
            z[j][0] + wh[0] <= u[0] + wl[1]
            z[j][I-1] + wl[I-1] <= u[I-1] + wh[I-2]
            for i in range(1,I-1):
                z[j][i] + wl[i] + wh[i] <= u[i] + wh[i-1] + wl[i+1] 

    def printSolution(self):
        def parent(j): return self.nodes[j]['parent']
        def name(k,i): return self.planes[k]['classes'][i]['name']

        K, O, I = len(self.planes), len(self.options[0]), len(self.options[0][0])
        x, y, v = self.x, self.y, self.v
        
        k0, o0 = 0, 0
        for k in range(K):
            if v[k].val > 0.5:
                k0 = k
                break
        for o in range(O):
            if y[0][o].val > 0.5:
                o0 = o
                break
        
        print('Optimal objective value: {:.4f}'.format(self.getObjVal()))
        print('Plane to hire: {!r}'.format(k0+1))
        print('In Period 1 use price option {!r}, and sell:'.format(o+1))
        for j in range(1,self.n0):
            if parent(j) != 0: break
            print('\tif scenario {!r} happens:'.format(j))
            for i in range(I):
                print('\t\t{!r} tickets of {!s} class'.format((int)(x[j][o][i].val),name(k0,i)))


