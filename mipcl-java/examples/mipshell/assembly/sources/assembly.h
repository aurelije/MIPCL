#include <mipshell.h>

class Cassembly: public CProblem
{
	int m_iStNum, m_iOpNum, m_iRelNum, m_iCycleTime;
	int *m_ipTime, *m_ipPrec, *m_ipSucc;
public:
	Cassembly(const char* name);
#ifndef __ONE_THREAD_
	Cassembly(const Cassembly &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cassembly();

	int model();
	
	void readData(const char* fileName);
	void printSol(VAR_VECTOR &x);

};

