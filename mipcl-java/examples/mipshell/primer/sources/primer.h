#include <mipshell.h>

class Cprimer: public CProblem
{
// TODO: declare MIPshell variables
public:
	Cprimer(const char* name);
#ifndef __ONE_THREAD_
	Cprimer(const Cprimer &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cprimer();

	int model();
// TODO: declare problem specific functions

};

