#include <mipshell.h>

class Cufl: public CProblem
{
	int q; // maximum number of facilities
	INT_VECTOR c; // cost vector
	INT_VECTOR f; // vector of fixed costs
	INT_VECTOR b; // facility capacities
public:
	Cufl(const char* name);
#ifdef __THREADS_
	Cufl(const Cufl &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cufl();

	void readUFL(const char* name);
	int model();
};

