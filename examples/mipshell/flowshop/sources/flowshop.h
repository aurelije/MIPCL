#include <mipshell.h>

class Cflowshop: public CProblem
{
	INT_VECTOR p; // processing times
public:
	Cflowshop(const char* name);
#ifdef __THREADS_
	Cflowshop(const Cflowshop &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cflowshop();

	int model();
// TODO: declare problem specific functions

};

