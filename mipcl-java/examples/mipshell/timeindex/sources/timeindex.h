#include <mipshell.h>

class Ctimeindex: public CProblem
{
	int m_iJobNum, m_iMachineNum,
		*m_ipRelease, *m_ipDead, *m_ipProcTime, *m_ipStart;
	double *m_dpWeight; // weights
public:
	Ctimeindex(const char* name);
#ifndef __ONE_THREAD_
	Ctimeindex(const Ctimeindex &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Ctimeindex();

	int model();

	void readData(const char* fileName);
	void printSol(VAR_ARRAY &x);
};

