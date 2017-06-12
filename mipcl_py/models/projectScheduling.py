from mipcl_py.mipshell.mipshell import *

class projectSchedule(Problem):
    def __init__(self, project):
        Problem.__init__(self, project['Name'])
        self.project = project

    def cpm(self, H):
        jobs, renRes = self.project['Jobs'], self.project['renRes']

        for job in jobs:
            es = 0
            for j in job['Prec']:
                ef = jobs[j]['EF']
                if es < ef:
                    es = ef
            job['EF'] = es + min(M['Duration'] for M in job['Modes'])
            job['LF'] = H

        for job in reversed(jobs):
            ls = job['LF'] - min(M['Duration'] for M in job['Modes'])
            for j in job['Prec']:
                if jobs[j]['LF'] > ls:
                    jobs[j]['LF'] = ls

    def model(self):    
        renRes, nonRenRes = self.project['renRes'], self.project['nonRenRes']
        jobs = self.project['Jobs']
        H = self.project['Horizon']
        self.cpm(H)
 
        T = Var('T',INT)
        e = VarVector([len(jobs)],'e',INT)
        d = VarVector([len(jobs)],'d',INT)
        self.x = x = []
        for j,job in enumerate(jobs):
            ef, lf = job['EF'], job['LF']+1
            x.append([])
            for m in range(len(job['Modes'])):
                x[j].append({})
                for t in range(ef,lf):
                    x[j][m][t] = Var('x({:d},{:d},{:d})'.format(j,m,t),BIN)

        minimize(T)

        for j,job in enumerate(jobs):
            ef, lf = job['EF'], job['LF']+1
# each job starts only in one mode and finishes only once 
            sum_(x[j][m][t] for m in range(len(job['Modes'])) for t in range(ef,lf)) == 1
            d[j] == sum_(M['Duration']*x[j][m][t] for m,M in enumerate(job['Modes'])\
                                                  for t in range(ef,lf))
            e[j] == sum_(t*x[j][m][t] for m in range(len(job['Modes'])) for t in range(ef,lf))
            T >= e[j]

# renewable resource limits
        for tau in range(1,H+1):
            for r,R in enumerate(renRes):
                sum_(M['renRes'][r]*x[j][m][t] for j,job in enumerate(jobs)\
                    for m,M in enumerate(job['Modes'])\
                    for t in range(max(tau,job['EF']),min(tau+M['Duration'],job['LF'])+1)) <= R['Quantity']

# nonrenewable resource limits
        for r,R in enumerate(nonRenRes):
            sum_(M['nonRenRes'][r]*x[j][m][t] for j,job in enumerate(jobs)\
                for m,M in enumerate(job['Modes'])\
                    for t in range(job['EF'],job['LF']+1)) <= R['Quantity']

# precedence relations
        for j2,job in enumerate(jobs):
            if len(job['Prec']) > 0:
            	for j1 in job['Prec']:
                	e[j2] - e[j1] >= d[j2]
            else: # j2 has not predecessors
                e[j2] >= d[j2]

    def getSchedule(self):
        schedule = None
        if self.is_solution is not  None:
            if self.is_solution == True:
                jobs = self.project['Jobs']
                qr = len(self.project['renRes'])
                x = self.x
                schedule = {}
                schedule['MakeSpan'] = T = int(self.getObjVal() + 0.5)
                schedule['renRes'] = renRes = [[0 for i in range(qr)] for t in range(T+1)]
                schedule['Mode'] = mode =[]
                schedule['Start'] = start = []
                schedule['End'] = end =[]
                for j,job in enumerate(jobs):
                    stop = False
                    for m,M in enumerate(job['Modes']):
                        for t in range(job['EF'],job['LF']+1):
                            if x[j][m][t].val > 0.5:
                                e = t
                                s = e - M['Duration']
                                mode.append(m)
                                start.append(s)
                                end.append(e)
                                for tau in range(s,e+1):
                                    for r in range(qr):
                                        renRes[tau][r] += M['renRes'][r]
                                stop = True
                                break
                        if stop:
                            break  
        return schedule

    def printSchedule(self, schedule):
        jobs = self.project['Jobs']
        qr = len(self.project['renRes'])
        mode, start, end = schedule['Mode'], schedule['Start'], schedule['End']
        rRes = schedule['renRes']
        print('Job schedule:')
        print(' _________________________________')
        print('|   Job  | Mode |  Start |    End |')
        print('|--------+------+--------+--------|')
        for j, job in enumerate(jobs):
            print('| {!s} | {:4d} | {:6d} | {:6d} |'.format(job['Name'].rjust(6)[:6],mode[j],start[j],end[j]))
        print(' ---------------------------------')
#
        print('\nResource usage:')
        T = schedule['MakeSpan']
        print(' ____' + '_'*(9*qr))
        str = '|     t |'
        for R in self.project['renRes']:
            str += ' {!s} |'.format(R['Name'].rjust(5)[:5])
        print(str)
        print('|-------' + '+-------'*qr + '|')
        for t in range(1,T+1):
            str = '| {:5d} |'.format(t)
            for i in range(qr):
                str += ' {:5d} |'.format(rRes[t][i])
            print(str)
        print(' ----' + '-'*(9*qr))


