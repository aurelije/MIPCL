#include <mipshell.h>

class Cfl: public CProblem
{
	int m_iLocationNum, m_iSiteNum, m_iQ;
	int *m_ipPopulation, *m_ipCapacity, *m_ipFixedCost, *m_ipCost;

public:
	Cfl(const char* name);
#ifndef __ONE_THREAD_
	Cfl(const Cfl &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cfl();

	int model();
	
	void readData(const char *fileName);
	void printSol(VAR_VECTOR &x, VAR_VECTOR &y);
};

