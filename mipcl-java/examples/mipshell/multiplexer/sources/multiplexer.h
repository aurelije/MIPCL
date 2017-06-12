#include <mipshell.h>

class Cmultiplexer: public CProblem
{
// TODO: declare MIPshell variables
	int m_iNodeNum, m_iRingNum, m_iRingCap; 
	int m_iMultCost, m_iSlotNum, m_iCardNum;
	int *m_ipCardCost, *m_ipCardCap;
	REAL_ARRAY *m_paDemand;
public:
	Cmultiplexer(const char *name);
#ifndef __ONE_THREAD_
	Cmultiplexer(const Cmultiplexer &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cmultiplexer();

	int model();
	
	void readData(const char *fileName);
//	void printSol(VAR_VECTOR &x, VAR_VECTOR &s);
};

