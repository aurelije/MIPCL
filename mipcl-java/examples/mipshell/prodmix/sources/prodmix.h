#include <mipshell.h>

class Cprodmix: public CProblem
{
// TODO: declare MIPshell variables
public:
	Cprodmix(const char* name);
#ifndef __ONE_THREAD_
	Cprodmix(const Cprodmix &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cprodmix();

	int model();
// TODO: declare problem specific functions

};

