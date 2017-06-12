from mipcl_py.mipshell.mipshell import *

class Packing(Problem):
    def prepare(self, factor, up):
        up = self.up

        LR, Vol = [], 1.0
        for s in self.instance['Cont. Sizes']:
            sr = s // factor
            if up:
            	Vol = Vol * sr
            else:
                Vol = Vol * s
            LR.append(sr)
        self.instance['Volume'] = Vol
        self.instance['Cont. rSizes'] = LR

        for item in self.instance['Items']:
            lr, v = [], 1.0
            for s in item['Sizes']:
                sr = s // factor
                if up:
                    if sr * factor < s: sr += 1
                    v = v * sr
                else:
                    v = v * s;
                lr.append(sr)
            item['Volume'] = v
            item['rSizes'] = lr

    def __init__(self, name, instance, factor=1, up=True):
        Problem.__init__(self,name)
        self.instance = instance
        self.factor = factor
        self.up = up
        self.prepare(factor,up)

    def model(self):
        def c(r): return items[r]['Cost']
        def vol(r): return items[r]['Volume']
        def l(r,i): return items[r]['rSizes'][i]
        def l1(r,i):
            if up:
                return items[r]['rSizes'][i]
            else:
                return items[r]['Sizes'][i]

        instance = self.instance
        L = instance['Cont. rSizes']
        up = self.up
        if up: L1 = L
        else: L1 = instance['Cont. Sizes']
        items = instance['Items']
        Vol = instance['Volume']
        m, n = len(L), len(items)

        self.x = x = []
        s, y = [], []
        self.z = z = VarVector([n],'z',BIN)#,priority=10)
        for r in range(n):
            x.append([])
            y.append([])
            for i in range(m):
                x[r].append(VarVector([L[i]],'x[{:d}][{:d}]'.format(r,i),BIN))
                y[r].append({})
                for j in range(l(r,i),L[i]+1):
                    y[r][i][j] = Var('y({:d},{:d},{:d})'.format(r,i,j),BIN)#,priority=3)
            s.append({})
            for r1 in range(r+1,n):
                s[r][r1] = {}
                for i in range(m):
                    s[r][r1][i] = Var('s({:d},{:d},{:d})'.format(r,r1,i),BIN)#,priority=5)

# (e[r][0],...,e[r,m-1]) upper-right corner of rectangle r
        e = VarVector([n,m],'e')

        maximize(sum_(c(r)*z[r] for r in range(n)) - sum_((0.0001*i)*e[r][i] for r in range(n) for i in range(m)))

        for r in range(n):
            for i in range(m):
                e[r][i] == sum_(j*y[r][i][j] for j in range(l(r,i),L[i]+1))
                for j in range(L[i]):
#                    x[r][i][j] <= z[r]
# x[r][i] must be one-humped
                    x[r][i][j] == sum_(y[r][i][j1] for j1 in range(max(j+1,l(r,i)),min(L[i],j+l(r,i))+1))
#                for j in range(l(r,i),L[i]):
#                    x[r][i][j-1] - x[r][i][j] <= y[r][i][j]
#                y[r][i][L[i]] == x[r][i][L[i]-1]
                sum_(y[r][i][j] for j in range(l(r,i),L[i]+1)) == z[r]

#                for j1 in range(L[i]-2):
#                    for j2 in range(j1+2,L[i]):
#                        x[r][i][j1] - x[r][i][j1+1] + x[r][i][j2] <= 1

# restrictions on the item sizes
                sum_(x[r][i][j] for j in range(L[i])) == l(r,i)*z[r]

# each pair of items, (r,r1), must be separated by a hyperplane which is perpendicular to one of m axes
            for r1 in range(r+1,n):
                sum_(s[r][r1][i] for i in range(m)) >= z[r] + z[r1] - 1
                q = 0
                for i in range(m):
                    if l(r,i) + l(r1,i) <= L[i]:
                        for j in range(L[i]):
                            x[r][i][j] + x[r1][i][j] + s[r][r1][i] <= z[r] + z[r1]
                    else:
                        s[r][r1][i] == 0
                        q += 1
                if q == m:
                    z[r] + z[r1] <= 1

# Less size and bigger cost => allocate first
                q, q1 = 0, 0  
                if c(r) >= c(r1): q += 1
                if c(r) <= c(r1): q1 += 1
                for i in range(m):
                    if l(r,i) <= l(r1,i): q += 1
                    if l(r,i) >= l(r1,i): q1 += 1
                if q == m+1:
                    z[r] >= z[r1]
                    if q1 == m+1: # r and r1 are identical
                        sum_((m-i)*e[r][i] for i in range(m)) >= sum_((m-i)*e[r1][i] for i in range(m))
                elif q1 == m+1:
                    z[r] <= z[r1]
# volume knapsack
        sum_(vol(r)*z[r] for r in range(n)) <= Vol
# strip knapsacks
        for i in range(m): 
            for j in range(0,L[i],10):
                sum_((vol(r) // l1(r,i))*x[r][i][j] for r in range(n) if l1(r,i) > 0) <= Vol // L1[i]

# breaking central symmentries
#        for i in range(m):
#            L0 = L[i]
#            k1 = L0 // 2
#            if 2*k1  < L0:
#                k2 = k1 + 1
#            else:
#                k2 = k1 
#            sum_((L0-j)*x[r][i][j] for r in range(n) for j in range(k1)) >= \
#            sum_((j+1)*x[r][i][j] for r in range(n) for j in range(k2,L0))

    def getLayout(self):
        factor = self.factor
        L = self.instance['Cont. rSizes']
        L1 = self.instance['Cont. Sizes']
        items = self.instance['Items']
        m, n = len(L), len(items)
        x, z = self.x, self.z
        layout = {'Cost': int(self.getObjVal() + 0.5), 'Positions': {}}
        positions = layout['Positions']
     
        for r in range(n):
            if z[r].val > 0.5:
                origin = positions[r] = []
                for i in range(m):
                    for j in range(L[i]):
                        if x[r][i][j].val > 0.5:
                            origin.append(j * factor)
                            break
        return layout

    def printLayout(self):
        if self.is_solution is not None:
            if self.is_solution:      
                items = self.instance['Items']
                m = len(self.instance['Cont. Sizes'])
                layout = self.getLayout()
                positions = layout['Positions']
                print('Layout cost = {:d}'.format(layout['Cost']))
                for r in sorted(positions.keys()):
                    str0 = repr(items[r]['Type']).rjust(3) + '({:d}) : '.format(items[r]['Cost']).rjust(8)
                    str1, str2 = '', ''
                    for i,X in enumerate(positions[r]):
                        if i > 0:
                            str1 += ','
                            str2 += ','
                        str1 += repr(X)
                        str2 += repr(X + items[r]['Sizes'][i])
                    print(str0 + '[(' + str1 + '), (' + str2 + ')]')
            else:
                print('No feasible solution has been found')
        else:
            print('Please call optimize() first')

