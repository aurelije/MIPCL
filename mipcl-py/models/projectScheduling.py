import mipshell
from mipshell import *
from math import sqrt

class projectScheduling(Problem):
    def __init__(self, name, project):
        Problem.__init__(self,name)
        self.project = project

    def mcp(self, H):
        jobs, renRes = self.project['Jobs'], self.project['renRes']

        for job in jobs:
            es = 0
            for j in job['Prec']:
                ef = jobs[j]['EF']
                if es < ef:
                    es = ef
            job['EF'] = es + min(mode[0] for mode in job['Modes'])
            job['LF'] = H
#            print(repr(job['Job']) + ' early start at ' + repr(es))

        for job in reversed(jobs):
            ls = job['LF'] - min(mode[0] for mode in job['Modes'])
            for j in job['Prec']:
                if jobs[j]['LF'] > ls:
                    jobs[j]['LF'] = ls

    def makespanUpperBound(self):
        jobs, renRes = self.project['Jobs'], self.project['renRes']
        ef, p = [], []
        for job in jobs:
            p.append(job['Modes'][0][0])
            ef.append(0)
 
        tau, H = 0, 0
        cont = True
        while cont:
            cont = False
            H = 0
            for j2,job in enumerate(jobs):
                es = 0
                for j1 in job['Prec']:
                    if es < ef[j1]:
                        es = ef[j1]
                ef[j2] = es + p[j2]
                if ef[j2] > H:
                    H = ef[j2]
            for t in range(tau,H):
                for r,resAvail in enumerate(renRes):
                    useList, resUsed = [], 0
                    for j,job in enumerate(jobs):
                        d = job['Modes'][0]
                        if d[r+1]:
                            if t >= ef[j] - d[0] and t < ef[j]:
                                resUsed += d[r+1]
                                if t == ef[j] - d[0]:
                                    useList.append((d[0],j))
                    if resUsed > resAvail:
                        useList.sort()
                        while resUsed > resAvail:
                            d, j = useList.pop()
                            resUsed -= jobs[j]['Modes'][0][r+1]
                            p[j] += 1
                        cont = True
                        break
                if cont:
                    break
                else:
                    tau += 1
        print(repr(H))
        return H

    def model(self, H=0):    
        if not H:
            H = self.makespanUpperBound()
        self.mcp(H)
        renRes, nonrenRes = self.project['renRes'], self.project['nonrenRes']
        jobs = self.project['Jobs']
 
        T = Var('T')
        e, e2 = VarVector([len(jobs)],'e'), VarVector([len(jobs)],'e2')
        d = VarVector([len(jobs)],'d')
        self.x = x = []
        for j,job in enumerate(jobs):
            ef, lf = job['EF'], job['LF']
            x.append([])
            for m in range(len(job['Modes'])):
                x[j].append(VarArray(range(ef,lf),'x[{:d},{:d}]'.format(j,m),BIN))

        minimize(T)

        for j,job in enumerate(jobs):
# each job starts only in one mode and finishes only once 
            sum_(x[j][m][t] for m in range(len(job['Modes'])) for t in range(job['EF'],job['LF'])) == 1
            d[j] == sum_(M[0]*x[j][m][t] for m,M in enumerate(job['Modes'])\
                                      for t in range(job['EF'],job['LF']))
            e[j] == sum_((t+1)*x[j][m][t] for m in range(len(job['Modes'])) for t in range(job['EF'],job['LF']))
            e2[j] == sum_(((t+1)*(t+1))*x[j][m][t] for m in range(len(job['Modes'])) for t in range(job['EF'],job['LF']))
            T >= e2[j]

# renewable resource limits
        for tau in range(H):
            for r,R in enumerate(renRes):
                sum_(M[r+1]*x[j][m][t] for j,job in enumerate(jobs)\
                    for m,M in enumerate(job['Modes'])\
                    for t in range(max(tau,job['EF']),min(tau+M[0],job['LF']))) <= R

# nonrenewable resource limits
        q = len(renRes) + 1
        for r,R in enumerate(nonrenRes):
            sum_(M[r+q]*x[j][m][t] for j,job in enumerate(jobs)\
                for m,M in enumerate(job['Modes'])\
                    for t in range(job['EF'],job['LF'])) <= R

# precedence relations
        for j2,job in enumerate(jobs):
            for j1 in job['Prec']:
                e[j2] - e[j1] >= d[j2]

    def getSchedule(self):
        schedule = None
        if self.is_solution is not  None:
            if self.is_solution == True:
                jobs = self.project['Jobs']
                qr = len(self.project['renRes'])
                x = self.x
                schedule = {}
                schedule['MakeSpan'] = T = int(sqrt(self.getObjVal()) + 0.5)
                schedule['renRes'] = renRes = [[0 for i in range(qr)] for t in range(T)]
                schedule['Start'] = start = []
                schedule['End'] = end =[]
                for j,job in enumerate(jobs):
                    stop = False
                    for m,M in enumerate(job['Modes']):
                        for t in range(job['EF'],job['LF']):
                            if x[j][m][t].val > 0.5:
                                e = t + 1
                                s = e - M[0]
                                start.append(s)
                                end.append(e)
                                for tau in range(s,e):
                                    for r in range(qr):
                                        renRes[tau][r] += M[r+1]
                                stop = True
                                break
                        if stop:
                            break  
        return schedule

    def printSchedule(self, schedule):
        jobs = self.project['Jobs']
        qr = len(self.project['renRes'])
        start, end = schedule['Start'], schedule['End']
        renRes = schedule['renRes']
        print('Job schedule:')
        print(' ________________________')
        print('|  Job |  Start |    End |')
        print('|------+--------+--------|')
        for j, job in enumerate(jobs):
            print('| {:4d} | {:6d} | {:6d} |'.format(job['Job'],start[j],end[j]))
        print(' ------------------------')
#
        print('\nResource usage:')
        T = schedule['MakeSpan']
        print(' ___' + '_'*(9*qr))
        str = '|     t |'
        for i in range(qr):
            str += ' {:5d} |'.format(i+1)
        print(str)
        print('|-------' + '+-------'*qr) + '|'
        for t in range(T):
            str = '| {:5d} |'.format(t+1)
            for i in range(qr):
                str += ' {:5d} |'.format(renRes[t][i])
            print(str)
        print(' ---' + '-'*(9*qr))


