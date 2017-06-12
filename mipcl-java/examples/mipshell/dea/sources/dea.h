#include <mipshell.h>

class Cdea: public CProblem
{
	int m_iUnitNum, m_iResNum, m_iServiceNum;
	double *m_dpInOut; // only pointer
public:
	Cdea(const char* name, int unitNum, int resNum, int serviceNum, double *dpInOut);
#ifndef __ONE_THREAD_
	Cdea(const Cdea &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cdea();

	int model(int i0, double &rating,
			double *dpV, double *dpU, double *dpS, double *dpRes);	
};

