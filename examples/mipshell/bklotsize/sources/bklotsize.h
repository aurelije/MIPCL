#include <mipshell.h>

class Cbklotsize: public CProblem
{
	int m_iT, // # number of time periods
		m_iTp, // # number of production  periods
		*m_ipFxCost, *m_ipCapacity, *m_ipDemand, *m_ipCost;
public:
	Cbklotsize(const char* name);
#ifdef __THREADS_
	Cbklotsize(const Cbklotsize &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cbklotsize();

	int model();
// implementation
	void readData(const char* fileName);
	void printSol(VAR_VECTOR &x, VAR_VECTOR &y);
};

