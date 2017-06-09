#include <mipshell.h>

class Crefining: public CProblem
{
// TODO: declare MIPshell variables
public:
	Crefining(const char* name);
#ifdef __THREADS_
	Crefining(const Crefining &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Crefining();

	int model();
// TODO: declare problem specific functions

};

