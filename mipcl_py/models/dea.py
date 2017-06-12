from mipcl_py.mipshell.mipshell import *

class Dea(Problem):
    def model(self,i0,rs):
        """DEA.
        Input parameters:
            i0: integer, service unit to be estimated;
            rs: list of pairs of tuples, where, for one period, service unit i
	        uses r(i,j) = rs[i][0][j] units of resource j,
                and provides s(i,k) = rs[i][1][k] services of type k.
        """
        def r(i,j):
            return rs[i][0][j]
        def s(i,k):
            return rs[i][1][k]

        self.rs = rs
        n, m, l = len(rs), len(rs[0][0]), len(rs[0][1])
        
        self.i0 = i0 = i0-1
        self.u = u = VarVector([l],"u")
        self.v = v = VarVector([m],"v")
		
        maximize(sum_(s(i0,k)*u[k] for k in range(l)))
	
        sum_(r(i0,j)*v[j] for j in range(m)) == 1
	
        for i in range(n):
            if i != i0:
                sum_(s(i,k)*u[k] for k in range(l))\
             <= sum_(r(i,j)*v[j] for j in range(m))

    def printSolution(self):
        def r(i,j):
            return rs[i][0][j]
        def s(i,k):
            return rs[i][1][k]

        u, v, rs = self.u, self.v, self.rs
        n, m, l = len(rs), len(v), len(u)

        print('=> Unit {!r} of rating {:.4f}'.format(self.i0+1,self.getObjVal()))
        str = 'v = ('
        for j in range(m-1):
            str += '{:.4f}, '.format(v[j].val)
        print(str + '{:.4f})'.format(v[m-1].val))
        str = 'u = ('
        for k in range(l-1):
            str += '{:.4f}, '.format(u[k].val)
        print(str + '{:.4f})'.format(u[l-1].val))
         
        print(' _____________________________________')
        print('| Unit | Relative |  Weighted sum of  |')
        print('|      |  rating  | inputs  | outputs |')
        print('|------+----------+---------+---------|')
        for i in range(n):
            Ri = 0.0
            for j in range(m):
                Ri += r(i,j)*v[j].val
            Si = 0.0
            for k in range(l):
                Si += s(i,k)*u[k].val    
            print('| {:4d} |  {:6.4f}  | {:7.4f} | {:7.4f} |'.format(i+1,Si/Ri,Ri,Si))
        print(' -------------------------------------')

