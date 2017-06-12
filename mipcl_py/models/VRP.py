from mipcl_py.mipshell.mipshell import *
from math import sqrt

class VRP(Problem):
    def __init__(self, name, instance):
        Problem.__init__(self,name)
        self.instance = instance

    def model(self):
        def d(j): return customer[j]['Demand']
        def q(k): return vehicleType[k]['Quantity'] 
        def u(k): return vehicleType[k]['Capacity'] 
        def c(k,i,j):
            d = customer[i]['Coord'][0]-customer[j]['Coord'][0]
            cost = d*d
            d = customer[i]['Coord'][1]-customer[j]['Coord'][1]
            cost += d*d
            cost = round(sqrt(cost))
            cost *= vehicleType[k]['UnitDistCost']
            if i == 0:
                cost += vehicleType[k]['FixedCost']
            return cost

        vehicleType = self.instance['VehicleTypes']
        customer = self.instance['Customers']
        K, n = len(vehicleType), len(customer)

        self.x = x = VarVector((K,n,n),"x",BIN)
        y = VarVector((n,n),"y",INT)

        minimize(sum_(c(k,i,j)*x[k][i][j] for k in range(K) for i in range(n) for j in range(n)))

        for k in range(K):
            sum_(x[k][0][j] for j in range(1,n)) <= q(k)
            for j in range(1,n):
                sum_(x[k][i][j] for i in range(n)) == 1
                sum_(x[k][i][j] for i in range(n)) - sum_(x[k][j][i] for i in range(n)) == 0
            for i in range(n):
                x[k][i][i] == 0

        for j in range(1,n):
            sum_(y[i][j] for i in range(n)) - sum_(y[j][i] for i in range(n)) == d(j)
            for i in range(n):
                y[i][j] <= sum_((u(k)-d(i))*x[k][i][j] for k in range(K))


    def setSolution(self):
        x = self.x
        K, n = len(x), len(x[0])

        self.first = first = []
        self.next = next = [0 for i in range(n)]
        
        for k in range(K):
            for j in range(1,n):
                if x[k][0][j].val > 0.5:
                    first.append((k,j))
                for i in range(1,n):
                    if x[k][i][j].val > 0.5:
                        next[i] = j
                        break

    def printSolution(self):
        vehicleType = self.instance['VehicleTypes']
        customer = self.instance['Customers']
        first, next = self.first, self.next

        print('Objective value: {0:.2f}\n'.format(self.getObjVal()))

        for r,(k,i) in enumerate(first):
            str = '\t0'
            load = 0
            while True:
                str += ' -> {:d}'.format(i)
                if i > 0:
                    load += customer[i]['Demand']
                    i = next[i]
                else: break
            print('Route {:d}, vehicle of type {!s}, cargo weight - {:d}'.\
                  format(r+1,vehicleType[first[r][0]]['Name'],load))
            print(str)

