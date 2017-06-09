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
#ifdef __THREADS_
	Cmultlotsize(const Cmultlotsize &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cmultlotsize();
	
	void readData(const char* fileName);
	int model();
};

