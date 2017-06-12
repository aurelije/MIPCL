#include <mipshell.h>

class Cmultlotsize: public CProblem
{
	int m_iProdNum, m_iMachTypeNum,m_iT;
	int *m_ipDur, *m_ipDemand,  *m_ipMachNum,
		*m_ipStockCap, *m_ipInitStock, *m_ipFinalStock;
	double *m_dpCost, *m_dpFixedCost, *m_dpHoldingCost,
		*m_dpTau, *m_dpRho;
public:
	Cmultlotsize(const char* name);
	Cmultlotsize(const char* name,
		int n, int m, int T,
		int *l, int *mch,
		int *Tmin, int *Tmax,
		double *f, double *c, double *h,
		double *rho, double *tau,
		int *d, int *si, int *sf
	);
#ifndef __ONE_THREAD_
	Cmultlotsize(const Cmultlotsize &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cmultlotsize();
	
	void readData(const char* fileName);
	int model();
};

