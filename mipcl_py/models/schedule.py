from mipcl_py.mipshell.mipshell import *

class Schedule(Problem):
    def model(self,jobs):
        """Sheduling Problem (m|r_j,d_j|\sum w_j U_j).

        Input parameters:
          jobs: list of size n, where, for each job j,
	     r(j)=jobs[j][0] and d(j)=jobs[j][1] are release and due dates;
             w(j)=jobs[j][2] weight;
	     jobs[j][3] is a dictionary, where
   	         M(j)=jobs[j][3].keys() is a subset of machines that are able to process job j; 
   	         p(j,i)=jobs[j][3][i] is processing time of job j on machine i;
                 prec(j)=jobs[j][4] is a subset of preceding jobs.
        """
        def r(j):
            return jobs[j][0]
        def d(j):
            return jobs[j][1]
        def w(j):
            return jobs[j][2]
        def p(j,i):
            return jobs[j][3][i]
        def M(j):
            return jobs[j][3].keys()
        def prec(j):
            return jobs[j][4]

        self.jobs = jobs
        n = len(jobs)
        self.m = m = 1 + max(i for j in range(n) for i in M(j))
        Rmin = min(r(j) for j in range(n))
        Dmax = 1 + max(d(j) for j in range(n))

        s = VarVector([n],'s')
        pt = VarVector([n],'pt')
        y = VarVector([n],'y',BIN)
        self.x = x = {}
        for j in range(n):
            for i in M(j):
                for t in range(r(j),d(j)-p(j,i)+1):
                    x[j,i,t] = Var('x' + str((j,i,t)),BIN)
	
        maximize(sum_(w(j)*y[j] for j in range(n)))

        for i in range(m):
            for t in range(Rmin,Dmax):
                sum_(x[j,i,tau] for j in range(n) if i in M(j)\
                    for tau in range(max(t-p(j,i),r(j)),min(t+1,d(j)-p(j,i)+1))) <= 1
	
        for j in range(n):
            y[j] == sum_(x[j,i,t] for i in M(j) for t in range(r(j),d(j)-p(j,i)+1))
            s[j] == sum_(t*x[j,i,t] for i in M(j) for t in range(r(j),d(j)-p(j,i)+1))
            pt[j] == sum_(p(j,i)*x[j,i,t] for i in M(j) for t in range(r(j),d(j)-p(j,i)+1))
            for j1 in prec(j):
                y[j] <= y[j1]
                s[j] >= s[j1] + pt[j1]
	
    def printSolution(self):
        def r(j):
            return jobs[j][0]
        def d(j):
            return jobs[j][1]
        def p(j,i):
            return jobs[j][3][i]
        def M(j):
            return jobs[j][3].keys()

        jobs=self.jobs
        x = self.x
        n = len(jobs)
                        
        print('Sheduling cost  = {:.4f}'.format(self.getObjVal()))
        print(' ------------------------------------')
        print('|  Job | Processor | Starts |   Ends |')
        print('+------------------------------------+')
        for j in range(n):
            for i in M(j):
                for t in range(r(j),d(j)-p(j,i)+1):
                    if x[j,i,t].val > 0.5:
                        print('| {:4d} | {:9d} | {:6d} | {:6d} |'.format(j+1,i+1,t,t+p(j,i)))
                        break
        print(' ------------------------------------')

