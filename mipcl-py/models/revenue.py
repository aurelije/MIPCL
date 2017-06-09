import mipshell
from mipshell import *

class Revenue(Problem):
    def model(self,planes,options,nodes):
        """Revenue (Yield) Management.
        Input parameters:
          planes: list of plane types, where planes[k] is structured as follows:
            s(k)=planes[k][0]: number of planes of type $k$;
            q1(k)=planes[k][2][0], q2(k)=planes[k][3][0], q3(k)=planes[k][4][0]:
	          number of places, resp., of first, business, and economy classes; 
            r1(k)=planes[k][2][1], r2(k)=planes[k][3][1], r3(k)=planes[k][4][1]:
	          number of seats, resp., of first, business, and economy classes
                  that can be transformed into seats of adjacent classes;
            f(k)=planes[k][1]: cost to hire one plane;	  
          options: list of size T, where
            options[t]: list (or tuple) of 3 options, where
              options[t][o]: 3-tuple that represents prices tickets for each of 3 classes;
          nodes: list of nodes of a scenario tree, each node j, except for node 0,
	         is characterized by a list (or tuple) nodes[j] which is structured as follows: 
            nodes[j][0]: index;
            nodes[j][1]: period;
            nodes[j][2]: index of the parent node;
            nodes[j][3]: probability of reaching this node from its parent;
            nodes[j][4]: 3-tuple, demands for tickets for each of three classes
                         if Option~1 is applied;
            nodes[j][5]: 3-tuple, demands for tickets for each of three classes
                         if Option~2 is applied;
      	    nodes[j][6]: 3-tuple, demands for tickets for each of three classes
                         if Option~3 is applied.   	 
        """
        def c(j,o,i):
            return nodes[j][3]*options[nodes[j][1]-1][o][i]
        def d(j,i,o):
            return nodes[j][o+4][i]
        def parent(j):
            return nodes[j][2]
        def s(k):
            return planes[k][0]
        def q1(k):
            return planes[k][2][0]
        def q2(k):
            return planes[k][3][0]
        def q3(k):
            return planes[k][4][0]
        def r1(k):
            return planes[k][2][1]
        def r2(k):
            return planes[k][3][1]
        def r3(k):
            return planes[k][4][1]
        def f(k):
            return planes[k][1]

        self.planes, self.options, self.nodes = planes, options, nodes
        K, T, O, I = len(planes), len(options), len(options[0]), len(options[0][0])
        n = len(nodes)
        for j in range(1,n):
            if nodes[j][1] == T:
                n0 = j
                break
        x = VarVector([n,O,I],"x",INT) # x[0] is not used
        self.y = y = VarVector([n0,O],"y",BIN)
        z = VarVector([n,I],"z",INT)
        self.v = v = VarVector([K],"v",INT)

        maximize(
	   sum_(c(j,o,i)*x[j][o][i] for j in range(1,n) for o in range(O) for i in range(I))\
	   - sum_(f(k)*v[k] for k in range(K))\
        )
	
        for j in range(n0):
            sum_(y[j][o] for o in range(O)) == 1;

        for j in range(1,n):
            for o in range(O):
                for i in range(I):
                    x[j][o][i] <= d(j,o,i)*y[parent(j)][o]
		
        z[0][0] == 0
        z[0][1] == 0
        z[0][2] == 0
	
        for j in range(1,n):
            for i in range(I):
                z[j][i] == z[parent(j)][i] + sum_(x[j][o][i] for o in range(O))
		
        for j in range(n0,n): # it is assumed that I == 3; to be modifief in general case
            z[j][0] <= sum_((q1(k) + (r2(k)*q2(k))/100)*v[k] for k in range(K))
            z[j][1] <= sum_((q2(k) + (r1(k)*q1(k)+r3(k)*q3(k))/100)*v[k] for k in range(K))
            z[j][2] <= sum_((q3(k) + (r2(k)*q2(k))/100)*v[k] for k in range(K))
            z[j][0] + z[j][2] <= sum_((q1(k)+q3(k)+(r2(k)*q2(k))/100)*v[k] for k in range(K))
            z[j][0] + z[j][1] + z[j][2] <= sum_((q1(k)+q2(k)+q3(k))*v[k] for k in range(K))
	
        for k in range(K):
            v[k] <= s(k)

    def printSolution(self):
        K, O = len(self.planes), len(self.options[0])
        y, v = self.y, self.v
        
        print('Optimal objective value: {:.4f}'.format(self.getObjVal()))
        print('Planes to use:')
        print(' ' + '_'*(10+5*K))
        str = '|   Type   |'
        for k in range(K):
            str += ' {:2d} |'.format(k+1)
        print(str)
        print('|----------+' + '----+'*(K-1) + '----|')
        str = '| Quantity |'
        for k in range(K):
            str += ' {:2d} |'.format(int(v[k].val))
        print(str)
        print(' ' + '-'*(10+5*K))
        
        k = -1
        for o in range(O):
            if y[0][o].val > 0.5:
                k = o+1
                break
        print('\nPrice option in period 1: {:d}'.format(k))
 

