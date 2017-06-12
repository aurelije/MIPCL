#include <mipshell.h>

class Clotsize: public CProblem
{
	int m_iT, m_iInitStock;
	int *m_ipPrice, *m_ipFixedCost, *m_ipHoldCost, *m_ipCapacity, *m_ipDemand;
public:
	Clotsize(const char* name);
#ifndef __ONE_THREAD_
	Clotsize(const Clotsize &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Clotsize();

	int model();
	
	void readData(const char *fileName);
	void printSol(VAR_VECTOR &x, VAR_VECTOR &s);
};

