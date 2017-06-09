#include <mipshell.h>

class Cbatch: public CProblem
{
	int H;  
	INDEX_SET TASKS, STATES, UNITS;
	INDEX_SET_ARRAY K, Sin, Sout, Tin, Tout, I;
	REAL_ARRAY rho, u, z0, cs, cp, Vmin, Vmax; 
	INT_ARRAY p,dur;
public:
	Cbatch(const char* name);
#ifdef __THREADS_
	Cbatch(const Cbatch &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cbatch();

	void readSTN(const char *fileName);
	int model();
	void printSolution(VAR_ARRAY& y, VAR_ARRAY& z);
};

