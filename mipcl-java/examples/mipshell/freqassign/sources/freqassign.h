#include <mipshell.h>

class Cfreqassign: public CProblem
{
	int m_iK, m_iM, m_iN;
	int *m_ipStation1, *m_ipStation2, *m_ipInterference; 

// TODO: declare MIPshell variables
public:
	Cfreqassign(const char* name);
#ifndef __ONE_THREAD_
	Cfreqassign(const Cfreqassign &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cfreqassign();

	int model();

	void readData(const char *fileName);
	void printSol(VAR_VECTOR &f);
};

