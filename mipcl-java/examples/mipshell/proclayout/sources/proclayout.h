#include <mipshell.h>

class Cproclayout: public CProblem
{
	int m_iN, m_iM;
	int *m_ipMovingCost, *m_ipDist, *m_ipFlow;
public:
	Cproclayout(const char* name);
#ifndef __ONE_THREAD_
	Cproclayout(const Cproclayout &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cproclayout();

	int model();

	void readData(const char* fileName);
	void printSol(VAR_VECTOR &x);
	
	int c(int i,int j, int s, int r);
	int p(int i, int s) {return m_ipMovingCost[i*m_iM+s];}
};

