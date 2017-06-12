#include <mipshell.h>

class Cunitcom: public CProblem
{
	int m_iGenNum, m_iT;
	double m_dQ;
	int *m_ipDemand;
	int *m_ipUpCapacity, *m_ipLoCapacity,
		*m_ipRminus, *m_ipRPlus, 
		*m_ipStartUpCost, *m_ipFixedCost, *m_ipUnitCost;
public:
	Cunitcom(const char* name);
#ifndef __ONE_THREAD_
	Cunitcom(const Cunitcom &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cunitcom();

	int model();

	void readData(const char *fileName);
	void printSol(VAR_VECTOR &x);
};

