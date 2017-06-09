#include <mipshell.h>

class CIndexFund: public CProblem
{
	int m_iQ;
	REAL_VECTOR rho, Val;
public:
	CIndexFund(const char* name);
#ifdef __THREADS_
	CIndexFund(const CIndexFund &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~CIndexFund();

	int model();

	void readData(const char *fileName);
	void printSol(VAR_VECTOR &x, VAR_VECTOR &y);
};

