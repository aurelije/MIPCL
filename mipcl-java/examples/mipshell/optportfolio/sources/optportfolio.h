#include <mipshell.h>

class Coptportfolio: public CProblem
{
	double m_dTol, m_dZero, m_dV;
	int m_iN, m_iT, m_iQ;
	double  *m_dpRet, *m_dpMu, *m_dpP, *m_dpL;
	VAR_VECTOR *m_pY; // without allocating memory
public:
	Coptportfolio(const char* name);
#ifndef __ONE_THREAD_
	Coptportfolio(const Coptportfolio &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Coptportfolio();
//////
	void readData(const char* fileName);
	static double getMean(const double p, const int T, const double* dpR);
	void computeParameters(const double p);
//////
	int model();
	bool separate();
};

