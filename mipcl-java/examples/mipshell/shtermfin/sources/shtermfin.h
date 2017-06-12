#include <mipshell.h>

class Cshtermfin: public CProblem
{
	int m_iT, m_iSecurityNum, m_iCreditLineNum, *m_ipInitPortfolio;
	double m_dInitCash,
			*m_dpSaleCost, *m_dpPurchaseCost,
			*m_dpMinCash,*m_dpLoanCost, *m_dpDemand, *m_dpLineCapacity;
// TODO: declare MIPshell variables
public:
	Cshtermfin(const char* name);
#ifndef __ONE_THREAD_
	Cshtermfin(const Cshtermfin &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cshtermfin();

	int model();
	
	void readData(const char *fileName);
	void printSol(VAR_VECTOR &x, VAR_VECTOR &xs, VAR_VECTOR &xb, VAR_VECTOR &y, VAR_VECTOR &z);
};

