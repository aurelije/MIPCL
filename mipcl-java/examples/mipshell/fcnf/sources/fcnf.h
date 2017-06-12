#include <mipshell.h>

class Cfcnf: public CProblem
{
	int m_iVertNum, m_iEdgeNum,*m_ipTail,*m_ipHead;
	double *m_dpCapacity,*m_dpFixedCost,*m_dpCost,*m_dpDemand;
public:
	Cfcnf(const char* fileName);
#ifndef __ONE_THREAD_
	Cfcnf(const Cfcnf &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cfcnf();
////////////////////////// implementation
	int model();

	void printSolution(VAR_VECTOR &flow);
private:
	void readNet(const char* fileName);
};

