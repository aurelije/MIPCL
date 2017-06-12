#include "dea.h"
#include <cstring>
//#include <cmath>
//#include <iomanip>


Cdea::Cdea(const char* name,
		int unitNum, int resNum, int serviceNum, double *dpInOut):
			CProblem(name), 
			m_iUnitNum(unitNum), m_iResNum(resNum), m_iServiceNum(serviceNum),
			m_dpInOut(dpInOut)
{
}

#ifndef __ONE_THREAD_
Cdea::Cdea(const Cdea &other, int thread): CProblem(other,thread)
{
// TODO: write your code here
} // end of Cdea::Cdea(Cdea &other, int thread)

CMIP* Cdea::clone(const CMIP *pMip, int thread)
{
	return static_cast<CMIP*>(new Cdea(*static_cast<Cdea*>(const_cast<CMIP*>(pMip)),thread));
}
#endif

Cdea::~Cdea()
{
#ifndef __ONE_THREAD_
	if (!m_iThread) {
#endif
// TODO: delete shared members 
#ifndef __ONE_THREAD_
	}
#endif
// TODO: delete notshared members 
}

//////////////////// implementation

#include "dea.aux"

