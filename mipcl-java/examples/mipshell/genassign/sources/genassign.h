#include <mipshell.h>

class Cgenassign: public CProblem
{
	REAL_VECTOR l, c, p;
public:
	Cgenassign(const char* name);
#ifndef __ONE_THREAD_
	Cgenassign(const Cgenassign &other, int thread);
	CMIP* clone(const CMIP *pMip, int thread);
#endif
	virtual ~Cgenassign();

// implementation
	int model();
	void readData(const char* FileName);

};

